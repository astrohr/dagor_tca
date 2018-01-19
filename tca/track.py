#!/usr/bin/env python
"""Dagor tracking 1.1

Usage:
  track.py start
  track.py sync_console
  track.py sync
  motors.py [-h | --help | help]
  motors.py --version

Commands:
  start             Start dynamic tracking.
  speed             Set approximate tracking speed.
  sync_console      Position sync console, for fine position adjust using arrow keys.
  sync              Start interactive mode - allows to adjust position and sync celest coordinates

Options:
  -h --help         Show this screen.
  --quiet           Only data on stdout

"""
from __future__ import division
from collections import OrderedDict

from datetime import datetime
import random

from docopt import docopt
from time import time, sleep
import os
import sys

from common import NonBlockingConsole, BASE_PATH, EnterAbort, print_, wait_for_time, sign, p_, wait_for_stop
from formats import format_hours, format_degrees, parse_degrees, parse_hours
import motors as dagor_motors
import path as dagor_path
import position as dagor_position
import cat as dagor_catalog
import local.configuration as conf

TRACKING_COORDINATES_FILE = os.path.join(BASE_PATH, 'coords.txt')
TRACKING_CORRECTIONS_FILE = os.path.join(BASE_PATH, 'tracking_corrections.txt')
TRACKING_CORRECTIONS_STEP_HA = parse_degrees('0:01')
TRACKING_CORRECTIONS_STEP_DE = parse_hours('0h01m')

TRACKING_OK_TARGET_ZONE_HA = parse_degrees('00:00:01') / 15 / 2
TRACKING_OK_TARGET_ZONE_DE = parse_degrees('00:00:01') / 2
TRACKING_ROUGH_TARGET_ZONE_HA = parse_degrees('00:01:00') / 15
TRACKING_ROUGH_TARGET_ZONE_DE = parse_degrees('00:01:00')
STATIC_OK_TARGET_ZONE_HA = parse_degrees('00:00:10') / 15
STATIC_OK_TARGET_ZONE_DE = parse_degrees('00:00:10')


def read_coordinates_file():
    celest = {
        'ra': 0,
        'de': 0,
    }
    try:
        with open(TRACKING_COORDINATES_FILE) as g:
            celest['ra'], celest['de'] = g.readline().split(" ")
            celest['ra'] = parse_hours(celest['ra'])
            celest['de'] = parse_degrees(celest['de'])
            return celest
    except (IOError, ValueError):
        return None


def write_coordinates_file(celest):
    with open(TRACKING_COORDINATES_FILE, 'w') as f:
        line = "{ra} {de}".format(**celest)
        f.write(line)


def reset_coordinates_file():
    with open(TRACKING_COORDINATES_FILE, 'w') as f:
        f.write('')


def reset_correction_file():
    write_correction_file({'ha_offset': 0, 'de_offset': 0})


def write_correction_file(offsets):
    with open(TRACKING_CORRECTIONS_FILE) as f:
        lines = [line if line[0] == '#' else '{}\n' for line in f.readlines()]
    with open(TRACKING_CORRECTIONS_FILE, 'w') as f:
        f.write(''.join(lines).format(
            format_hours(offsets['ha_offset']),
            format_degrees(offsets['de_offset']),
        ))


def read_corrections_file():
    manual_corrections = {
        'ha_offset': 0,
        'de_offset': 0,
    }
    try:
        with open(TRACKING_CORRECTIONS_FILE) as f:
            lines = [line for line in f.readlines() if line[0] != '#']
    except IOError:
        lines = [0, 0]
        #raise
    if len(lines) > 1:
        manual_corrections['ha_offset'] = parse_hours(lines[0])
        manual_corrections['de_offset'] = parse_degrees(lines[1])
    return manual_corrections


def sync_console():
    with NonBlockingConsole() as nbc:
        print "\x1b[2J"
        print "\x1b[0;0HTracking sync console"
        print "\n\n"
        #reset_correction_file()
        manual_corrections = read_corrections_file()
        data = ''
        last_data = ''
        while True:
            sleep(0.05)
            if data:
                last_data = data
            data = nbc.get_data()
            print "\x1b[2;0H\x1b[J"
            print "Position correction:"
            print '  HA: {ha_offset}\n  DE: {de_offset}'.format(
                ha_offset=format_hours(manual_corrections['ha_offset']),
                de_offset=format_degrees(manual_corrections['de_offset']),
            )
            print "\nAvailable commands:"
            print "ARROWS : Nudge"
            print "SHIFT + ARROWS : Small nudge"
            print "CTRL + ARROWS : Big nudge, not working with Putty / Windows"
            print "SHIFT + S : Synchronize"
            print "SHIFT + T : Set new Target from catalog"
            print "ESCAPE : quit"
            print
            known_sequences = {
                '\x1b': 'ESCAPE',
                '\x1b[A': 'ARROW_UP',
                '\x1b[B': 'ARROW_DOWN',
                '\x1b[C': 'ARROW_RIGHT',
                '\x1b[D': 'ARROW_LEFT',
                '\x1b[1;2A': 'SHIFT_ARROW_UP',
                '\x1b[1;2B': 'SHIFT_ARROW_DOWN',
                '\x1b[1;2C': 'SHIFT_ARROW_LEFT',
                '\x1b[1;2D': 'SHIFT_ARROW_RIGHT',
                '\x1bOA': 'SHIFT_ARROW_UP',  # putty / win ?
                '\x1bOB': 'SHIFT_ARROW_DOWN',  # putty / win ?
                '\x1bOC': 'SHIFT_ARROW_LEFT',  # putty / win ?
                '\x1bOD': 'SHIFT_ARROW_RIGHT',  # putty / win ?
                '\x1b[1;5A': 'CTRL_ARROW_UP',
                '\x1b[1;5B': 'CTRL_ARROW_DOWN',
                '\x1b[1;5C': 'CTRL_ARROW_LEFT',
                '\x1b[1;5D': 'CTRL_ARROW_RIGHT',
                'S': 'SYNC',
                'R': 'RESET',
                'T': 'TARGET',
            }
            #print data.encode('string-escape') if data and data != '\x1b' else last_data.encode('string-escape')

            if data not in known_sequences.keys():
                continue
            if known_sequences[data] == 'ESCAPE':
                break
            elif known_sequences[data] == 'ARROW_UP':
                manual_corrections['de_offset'] -= TRACKING_CORRECTIONS_STEP_DE
            elif known_sequences[data] == 'ARROW_DOWN':
                manual_corrections['de_offset'] += TRACKING_CORRECTIONS_STEP_DE
            elif known_sequences[data] == 'ARROW_RIGHT':
                manual_corrections['ha_offset'] -= TRACKING_CORRECTIONS_STEP_HA
            elif known_sequences[data] == 'ARROW_LEFT':
                manual_corrections['ha_offset'] += TRACKING_CORRECTIONS_STEP_HA
            elif known_sequences[data] == 'ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACKING_CORRECTIONS_STEP_HA
            elif known_sequences[data] == 'SHIFT_ARROW_UP':
                manual_corrections['de_offset'] -= TRACKING_CORRECTIONS_STEP_DE / 6
            elif known_sequences[data] == 'SHIFT_ARROW_DOWN':
                manual_corrections['de_offset'] += TRACKING_CORRECTIONS_STEP_DE / 6
            elif known_sequences[data] == 'SHIFT_ARROW_RIGHT':
                manual_corrections['ha_offset'] += TRACKING_CORRECTIONS_STEP_HA / 20
            elif known_sequences[data] == 'SHIFT_ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACKING_CORRECTIONS_STEP_HA / 20
            elif known_sequences[data] == 'CTRL_ARROW_UP':
                manual_corrections['de_offset'] -= TRACKING_CORRECTIONS_STEP_DE * 10
            elif known_sequences[data] == 'CTRL_ARROW_DOWN':
                manual_corrections['de_offset'] += TRACKING_CORRECTIONS_STEP_DE * 10
            elif known_sequences[data] == 'CTRL_ARROW_RIGHT':
                manual_corrections['ha_offset'] += TRACKING_CORRECTIONS_STEP_HA * 10
            elif known_sequences[data] == 'CTRL_ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACKING_CORRECTIONS_STEP_HA * 10
            if 'ARROW' in known_sequences[data]:  # any arrow
                if abs(manual_corrections['ha_offset']) < TRACKING_CORRECTIONS_STEP_HA / 20:
                    manual_corrections['ha_offset'] = 0
                if abs(manual_corrections['de_offset']) < TRACKING_CORRECTIONS_STEP_DE / 20:
                    manual_corrections['de_offset'] = 0
                write_correction_file(manual_corrections)

            elif known_sequences[data] == 'SYNC':
                internal = dagor_position.get_internal()
                new_internal = {
                    'ha': internal['ha'] + manual_corrections['ha_offset'],
                    'de': internal['de'] + manual_corrections['de_offset'],
                }
                dagor_position.set_internal(new_internal)
                manual_corrections['ha_offset'] = 0
                manual_corrections['de_offset'] = 0
                reset_correction_file()

            elif known_sequences[data] == 'RESET':
                manual_corrections['ha_offset'] = 0
                manual_corrections['de_offset'] = 0
                reset_correction_file()

            elif known_sequences[data] == 'TARGET':
                os.system("stty echo")
                target = raw_input('Enter catalog name: ')
                try:
                    celest = dagor_catalog.get_celest(target)
                except ValueError:
                    celest = None
                    raw_input("Not found in catalog")
                if celest:
                    print celest
                    write_coordinates_file(celest)


class Target(object):
    TYPES = ('celest', 'altaz', )
    STATIC_DEFAULTS = {
        'celest': False,
        'altaz': True,
    }

    def __init__(self, coords, coords_type, static=None):
        self._celest = None
        self._altaz = None
        if coords_type not in self.TYPES:
            raise ValueError('{} not in Target.TYPES'.format(coords_type))
        if static is None:
            static = self.STATIC_DEFAULTS[coords_type]
        if static:
            if coords_type == 'celest':
                coords = dagor_position.celest_to_altaz(coords)
            self._altaz = coords
        else:
            if coords_type == 'altaz':
                coords = dagor_position.altaz_to_celest(coords)
            self._celest = coords

    def celest(self):
        if self._altaz:
            return dagor_position.altaz_to_celest(self._altaz)
        else:
            return self._celest


class Tracking(object):

    def __init__(self):
        self.current = {}
        self.config = {}
        self.tracking_check_interval = conf.TRACKING['tracking_check_interval']
        self.target = None

    DEFAULT_CONF = {
        'target_celest': None,
        'chirality': None,
        'target_is_static': False,
        'stop_on_target': False,
        'rough': False,
        'force': False,
    }

    def load_config(self, config):
        self.config = self.DEFAULT_CONF.copy()
        self.config.update(config)
        if self.config['target_celest'] is None:
            self.config['target_celest'] = dagor_position.get_celest()
        self.target = Target(
            self.config['target_celest'],
            'celest',
            self.config['target_is_static'])
        self._reset_current()

    def _reset_current(self):
        self.current = {}

    def init_motors(self):
        dagor_motors.init()
        dagor_motors.require_stopped('ha')
        dagor_motors.require_stopped('de')
        dagor_motors.enable('ha')
        dagor_motors.enable('de')
        dagor_motors.set_speed('ha', 0)
        dagor_motors.set_speed('de', 0)

    def adjust_speeds(self, ha_err, de_err, max_ha, max_de):
        """
        Calculate max possible speeds to equalize travel time.
        """
        t_ha = abs(ha_err) / max_ha
        t_de = abs(de_err) / max_de
        t = max(t_ha, t_de)
        max_ha = min(t_ha / t * max_ha, max_ha)
        max_de = min(t_de / t * max_de, max_de)
        return {
            'speed_ha': max_ha,
            'speed_de': max_de,
        }

    def slope(self, speed, err, a=9, b=400):
        """Calculate real speed given position error."""
        return (a * speed * abs(err) ** 2 / b) ** (1 / 3)

    def speed_real_to_motor(self, speed, limit):
        """Translate from real speed (deg/s or h/sec) to motor speed units."""
        factor = conf.MOTORS['speed_limit'] / limit
        return speed * factor

    def rnd_track_interval(self):
        """seconds to wait"""
        if not conf.TRACKING['enable_rnd']:
            return self.tracking_check_interval
        rnd_interval = self.tracking_check_interval / 1.1
        start = self.tracking_check_interval - rnd_interval
        end = self.tracking_check_interval + rnd_interval
        return random.uniform(start, end)

    def stop_tracking(self):
        print_("Stopping")
        dagor_motors.stop()
        wait_for_stop(dagor_motors.get_motor('ha'), dots=True)
        wait_for_stop(dagor_motors.get_motor('de'), dots=True)

    def update_path(self, path, current_internal):
        """Remove the first point if we are close enough to it"""
        if len(path) > 2:
            if dagor_position.angular_distance(
                    current_internal,
                    {'ha': path[1].ha, 'de': path[1].de}
            ) < 0.8:
                del path[1]
        return path

    def _loop_wait(self):
        self.current['waiting_interval'] = self.rnd_track_interval()
        wait_for_time(
            self.current['waiting_interval'],
            dots=True,
            enter_abort=True,
            interval=self.tracking_check_interval / 100,
        )

    def _loop_read_inputs(self):
        self.config['target_internal'] = dagor_position.celest_to_internal(
            self.target.celest(),
            self.config['chirality'],
        )
        self.current['internal'] = dagor_position.get_internal()
        self.current['t_now'] = time()
        self.current['now'] = datetime.utcnow()
        self.current['offset'] = read_corrections_file()

    def _loop_calculate_target(self):
        self.current['path'] = dagor_path.get_path(
            self.current['internal'],
            self.config['target_internal'],
            force=self.config['force'],
        )
        self.current['path'] = self.update_path(
            self.current['path'], self.current['internal'])
        self.current['target'] = self.current['path'][1]

    def _loop_check_space(self):
        hence_sec = 10 if self.current['on_target'] else 0
        if self.config['force']:
            space_ok = True
        else:
            space_ok = dagor_position.check_space(
                self.current['internal'],
                hence_sec,
            )
        if not space_ok:
            print_('End of allowed space!')
            raise EnterAbort()

    def _loop_calculate_errors(self):
        ha_now = self.current['internal']['ha'] + self.current['offset'][
            'ha_offset']
        de_now = self.current['internal']['de'] + self.current['offset'][
            'de_offset']
        self.current['ha_err'] = ha_now - self.current['target'].ha
        self.current['de_err'] = de_now - self.current['target'].de

    def _loop_calculate_speeds(self):
        # TODO get rid of dagor_motors.MAX_SPEED_HA and use conf.MOTORS['speed_limit'] instead
        speeds = self.adjust_speeds(
            self.current['ha_err'],
            self.current['de_err'],
            dagor_motors.MAX_SPEED_HA,
            dagor_motors.MAX_SPEED_DE,
        )
        b = 200 if self.config['rough'] else 400
        # HA:
        ha_speed = self.slope(speeds['speed_ha'], self.current['ha_err'], b=b)
        ha_speed = self.speed_real_to_motor(
            ha_speed, dagor_motors.MAX_SPEED_HA)
        ha_speed = min(
            ha_speed,
            conf.MOTORS['speed_limit'] * speeds[
                'speed_ha'] / dagor_motors.MAX_SPEED_HA)
        ha_speed = int(ha_speed * sign(self.current['ha_err']))
        if not self.config['target_is_static']:
            ha_speed += dagor_motors.TRACKING_SPEED
        # DE:
        de_speed = self.slope(speeds['speed_de'], self.current['de_err'], b=b)
        de_speed = self.speed_real_to_motor(
            de_speed, dagor_motors.MAX_SPEED_DE)
        de_speed = min(
            de_speed,
            conf.MOTORS['speed_limit'] * speeds[
                'speed_de'] / dagor_motors.MAX_SPEED_DE)
        de_speed = int(de_speed * sign(self.current['de_err']))

        # limit total speed:
        sum_speed = abs(ha_speed) + abs(de_speed)
        if sum_speed > conf.TRACKING['total_speed_limit']:
            correct_factor = conf.TRACKING['total_speed_limit'] / sum_speed
            ha_speed *= correct_factor
            de_speed *= correct_factor

        # all done:
        self.current.update(
            ha_speed=int(round(ha_speed)),
            de_speed=int(round(de_speed)),
        )

    def _loop_check_target(self):
        if self.config['rough']:
            if abs(self.current[
                       'ha_err']) < TRACKING_ROUGH_TARGET_ZONE_HA and abs(
                    self.current['de_err']) < TRACKING_ROUGH_TARGET_ZONE_DE:
                if not self.current['on_target_since']:
                    self.current['on_target_since'] = self.current['t_now']
            else:
                self.current['on_target_since'] = None
        elif self.config['target_is_static']:
            if abs(self.current['ha_err']) < STATIC_OK_TARGET_ZONE_HA and abs(
                    self.current['de_err']) < STATIC_OK_TARGET_ZONE_DE:
                if not self.current['on_target_since']:
                    self.current['on_target_since'] = self.current['t_now']
            else:
                self.current['on_target_since'] = None
        else:
            if abs(self.current[
                       'ha_err']) < TRACKING_OK_TARGET_ZONE_HA and abs(
                    self.current['de_err']) < TRACKING_OK_TARGET_ZONE_DE:
                if not self.current['on_target_since']:
                    self.current['on_target_since'] = self.current['t_now']
            else:
                self.current['on_target_since'] = None
        self.current['on_target'] = False
        if self.current['on_target_since']:
            time_on_target = time() - self.current['on_target_since']
            self.current['on_target'] = time_on_target > 3

    def _loop_stop_on_target(self):
        if self.config['stop_on_target'] and self.current['on_target']:
            raise EnterAbort()

    def _loop_apply_speeds(self):
        dagor_motors.set_speed('ha', self.current['ha_speed'])
        dagor_motors.set_speed('de', self.current['de_speed'])

    def track(self):
        if not self.config:
            raise RuntimeError('tracking not configured')

        self._reset_current()
        self.init_motors()

        self.current['on_target'] = False
        self.current['on_target_since'] = None
        try:
            while True:
                self._loop_wait()
                self._loop_read_inputs()
                self._loop_calculate_target()
                self._loop_check_space()
                self._loop_calculate_errors()
                self._loop_calculate_speeds()
                self._loop_check_target()
                self._loop_print_console_header()
                self._loop_print_stats()
                self._loop_apply_speeds()
                self._loop_stop_on_target()

        except dagor_path.NoPath as e:
            print_("No path: {}".format(e))
            print_(self.target.celest())

        except (EnterAbort, KeyboardInterrupt):
            # Note: KeyboardInterrupt will sometimes not get caught here, it's a thing
            print("")

        except Exception as e:
            print_(e.message)
            print_("Exception info:\n")
            raise

        finally:
            self.stop_tracking()

    def _loop_print_console_header(self):
        print_("\x1b[2J\x1b[0;0H")
        print_('DAGOR GOTO')
        print_(self.current['now'])
        print_('----------------')
        print_("Press Enter to stop at any time")
        print_("")

    def _loop_print_stats(self):

        od = OrderedDict()
        od['speeds'] = '{:>13}  {:>13}'.format(
            self.current['ha_speed'], self.current['de_speed'])
        od['errors'] = '{:>13}  {:>13}'.format(
            format_degrees(self.current['ha_err'] * 15),
            format_degrees(self.current['de_err']),
        )
        celest = dagor_position.get_celest()
        od['celest'] = '{:>13}  {:>13}'.format(
            format_hours(celest['ra']), format_degrees(celest['de']))
        altaz = dagor_position.get_altaz()
        od['altaz '] = '{:>13}  {:>13}'.format(
            format_degrees(altaz['alt']), format_degrees(altaz['az']))
        chiral = dagor_position.get_chirality(self.current['internal']['de'])
        od['chiral'] = chiral
        if self.config['chirality'] != chiral:
            od['chiral'] += '  ->  {}'.format(self.config['chirality'])
        od['mode  '] = 'precise' if not self.config['rough'] else 'quick'
        od['target'] = 'static' if self.config['target_is_static'] else 'sky'
        od['inter.'] = '{:0<13}  {:0<13}'.format(
            self.current['internal']['ha'],
            self.current['internal']['de'])
        for i, point in enumerate(self.current['path'][1:], start=1):
            od['path_{}'.format(i)] = '{:0<13}  {:0<13}'.format(
                point.ha, point.de)
        od['on_target'] = self.current['on_target']
        p_(od)


def speed_tracking(target_celest=None,
                   chirality=None,
                   target_is_static=False,
                   stop_on_target=False,
                   rough=False,
                   force=False):
    tracking = Tracking()
    tracking.load_config({
        'target_celest': target_celest,
        'chirality': chirality,
        'target_is_static': target_is_static,
        'stop_on_target': stop_on_target,
        'rough': rough,
        'force': force,
    })
    tracking.track()


# Run as CLI client


def _main(args):

    if args['start']:
        tracking = Tracking()
        tracking.track()

    if args['sync']:
        raise NotImplementedError()

    if args['sync_console']:
        sync_console()


if __name__ == '__main__':

    args = docopt(__doc__, version=__doc__.split("\n")[0], options_first=True)

    if len(sys.argv) == 1 or args['help']:
        print __doc__.strip()
        exit(0)

    try:
        _main(args)
    except:
        raise
        #exit_('ERROR')

