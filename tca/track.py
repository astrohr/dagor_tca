#!/usr/bin/env python
"""Dagor tracking 1.1

Usage:
  track.py start
  track.py run
  track.py sync_console
  track.py sync
  motors.py [-h | --help | help]
  motors.py --version

Commands:
  start             Start dynamic tracking.
  run               Run tracking engine but don't move the telescope.
  speed             Set approximate tracking speed.
  sync_console      Position sync console, for fine position adjust
                    using arrow keys.
  sync              Start interactive mode - allows to adjust position
                    and sync celest coordinates

Options:
  -h --help         Show this screen.
  --quiet           Only data on stdout

"""
from __future__ import division
from collections import OrderedDict

from datetime import datetime, timedelta
import json
import math
import random

from docopt import docopt
from time import time, sleep
import os
import sys

from jsonschema import ValidationError

from common import (NonBlockingConsole, BASE_PATH, EnterAbort, print_,
                    wait_for_time, sign, p_, wait_for_stop)
from formats import format_hours, format_degrees, parse_degrees, parse_hours
import motors as dagor_motors
import path as dagor_path
import position as dagor_position
import schemas as dagor_schemas
import local.configuration as conf

from logging_conf import get_logger

logger = get_logger(__file__)


TRACKING_CORRECTIONS_FILE = os.path.join(BASE_PATH, 'tracking_corrections.txt')
TRACK_CORRECT_STEP_HA = parse_degrees('0:01')
TRACK_CORRECT_STEP_DE = parse_hours('0h01m')

TRACKING_OK_TARGET_ZONE_HA = parse_degrees('00:00:06') / 15 / 1.5
TRACKING_OK_TARGET_ZONE_DE = parse_degrees('00:00:06') / 1.5
TRACKING_ROUGH_TARGET_ZONE_HA = parse_degrees('00:01:00') / 15
TRACKING_ROUGH_TARGET_ZONE_DE = parse_degrees('00:01:00')
STATIC_OK_TARGET_ZONE_HA = parse_degrees('00:00:10') / 15
STATIC_OK_TARGET_ZONE_DE = parse_degrees('00:00:10')
TARGET_SETTLE_WAIT = 2  # seconds
AT_HOME_ZONE = parse_degrees('00:10:00')
AT_PARK_ZONE = parse_degrees('00:10:00')


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
        # TODO log error
    if len(lines) > 1:
        manual_corrections['ha_offset'] = parse_hours(lines[0])
        manual_corrections['de_offset'] = parse_degrees(lines[1])
    return manual_corrections


def sync_console():
    with NonBlockingConsole() as nbc:
        print "\x1b[2J"
        print "\x1b[0;0HTracking sync console"
        print "\n\n"
        manual_corrections = read_corrections_file()
        while True:
            sleep(0.05)
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

            if data not in known_sequences.keys():
                continue
            if known_sequences[data] == 'ESCAPE':
                break
            elif known_sequences[data] == 'ARROW_UP':
                manual_corrections['de_offset'] -= TRACK_CORRECT_STEP_DE
            elif known_sequences[data] == 'ARROW_DOWN':
                manual_corrections['de_offset'] += TRACK_CORRECT_STEP_DE
            elif known_sequences[data] == 'ARROW_RIGHT':
                manual_corrections['ha_offset'] -= TRACK_CORRECT_STEP_HA
            elif known_sequences[data] == 'ARROW_LEFT':
                manual_corrections['ha_offset'] += TRACK_CORRECT_STEP_HA
            elif known_sequences[data] == 'ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACK_CORRECT_STEP_HA
            elif known_sequences[data] == 'SHIFT_ARROW_UP':
                manual_corrections['de_offset'] -= TRACK_CORRECT_STEP_DE / 6
            elif known_sequences[data] == 'SHIFT_ARROW_DOWN':
                manual_corrections['de_offset'] += TRACK_CORRECT_STEP_DE / 6
            elif known_sequences[data] == 'SHIFT_ARROW_RIGHT':
                manual_corrections['ha_offset'] += TRACK_CORRECT_STEP_HA / 20
            elif known_sequences[data] == 'SHIFT_ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACK_CORRECT_STEP_HA / 20
            elif known_sequences[data] == 'CTRL_ARROW_UP':
                manual_corrections['de_offset'] -= TRACK_CORRECT_STEP_DE * 10
            elif known_sequences[data] == 'CTRL_ARROW_DOWN':
                manual_corrections['de_offset'] += TRACK_CORRECT_STEP_DE * 10
            elif known_sequences[data] == 'CTRL_ARROW_RIGHT':
                manual_corrections['ha_offset'] += TRACK_CORRECT_STEP_HA * 10
            elif known_sequences[data] == 'CTRL_ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACK_CORRECT_STEP_HA * 10
            if 'ARROW' in known_sequences[data]:  # any arrow
                abs_ha = abs(manual_corrections['ha_offset'])
                if abs_ha < TRACK_CORRECT_STEP_HA / 20:
                    manual_corrections['ha_offset'] = 0
                abs_de = abs(manual_corrections['de_offset'])
                if abs_de < TRACK_CORRECT_STEP_DE / 20:
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

    def altaz(self):
        if self._celest:
            return dagor_position.celest_to_altaz(self._celest)
        else:
            return self._altaz

    def target_coords(self):
        key = 'celest' if self._celest else 'altaz'
        value = self._celest if self._celest else self._altaz
        return {key: value}


class Tracking(object):

    def __init__(self):
        self.current = {}
        self.config = {}
        self.tracking_check_interval = conf.TRACKING['tracking_check_interval']
        self.target = None

    DEFAULT_CONF = {
        'target_celest': None,
        'target_altaz': None,
        'target_home': False,
        'target_park': False,
        'chirality': None,
        'target_is_static': False,
        'stop_on_target': False,
        'rough': False,
        'force': False,
        'tracking': False,
    }
    schema = dagor_schemas.Schema('track')

    _last_conf = {}
    _slewing = False
    _last_target_coords = {}
    _last_ha_speed = None
    _last_de_speed = None
    _ha_speed_same_since = None
    _de_speed_same_since = None
    _ha_warp_speed = False
    _de_warp_speed = False

    def _dump_current_json(self):
        tmp_file = conf.TRACKING['tracking_current_file'] + '.tmp'
        real_file = conf.TRACKING['tracking_current_file']
        current = self.current.copy()
        current['path'] = [
            (p.ha, p.de) for p in current['path']
        ]
        current['now'] = str(current['now'])
        with open(tmp_file, 'w') as f:
            f.write(json.dumps(current, indent=4, sort_keys=True))
        os.rename(tmp_file, real_file)

    def _dump_config_json(self):
        if self.config == self._last_conf:
            return  # already dumped this
        tmp_file = conf.TRACKING['tracking_target_file'] + '.tmp'
        real_file = conf.TRACKING['tracking_target_file']
        self.schema.validate(self.config)
        with open(tmp_file, 'w') as f:
            f.write(json.dumps(self.config, indent=4, sort_keys=True))
        os.rename(tmp_file, real_file)

    def _load_config_json(self):
        with open(conf.TRACKING['tracking_target_file'], 'r') as f:
            content = f.read()
            try:
                self.config = json.loads(content)
                self.schema.validate(self.config)
            except ValueError:
                logger.exception('Error ')
                self.config = {}
                return

    def get_config(self):
        self._load_config_json()
        return self.config

    def _process_config(self):
        if self.config['target_home']:
            self.config['target_celest'] = None
            self.config['target_is_static'] = True
            self.config['target_altaz'] = dagor_position.HOME_ALTAZ
            self.config['chirality'] = dagor_position.HOME_CHIRALITY
            self.config['rough'] = True
        if self.config['target_park']:
            self.config['target_celest'] = None
            self.config['target_is_static'] = True
            self.config['target_altaz'] = dagor_position.PARK_ALTAZ
            self.config['chirality'] = dagor_position.PARK_CHIRALITY
            self.config['rough'] = True
        if self.config['target_altaz']:
            self.config['target_celest'] = dagor_position.altaz_to_celest(
                self.config['target_altaz'])
            self.config['target_is_static'] = True
        if self.config['target_celest'] is None:
            self.config['target_celest'] = dagor_position.get_celest()
            self.config['target_is_static'] = True
        self.target = Target(
            self.config['target_celest'],
            'celest',
            self.config['target_is_static'])
        if self.config['chirality'] is None:
            self.config['chirality'] = dagor_position.get_chirality()
        if self.config['chirality'] == dagor_position.CHIRAL_CLOSEST:
            target_local = dagor_position.celest_to_local(
                self.config['target_celest'])
            chirality = dagor_position.get_closest_chirality(target_local)
            self.config['chirality'] = chirality
        if self.target.target_coords() != self._last_target_coords:
            self._last_target_coords = self.target.target_coords()
            self._slewing = True
        # if (not self.config['target_celest'] and
        #         not self.config['target_altaz'] and
        #         not self.config['target_home'] and
        #         not self.config['target_park']):
        if not self.config["tracking"]:
            self._slewing = False

    def set_config(self, config):
        self.config = self.DEFAULT_CONF.copy()
        self.config.update(config)
        self._reset_current()
        self._process_config()
        self._dump_config_json()

    def _reset_current(self):
        self.current = {}

    @staticmethod
    def init_motors():
        dagor_motors.init()
        dagor_motors.require_stopped('ha')
        dagor_motors.require_stopped('de')
        dagor_motors.enable('ha')
        dagor_motors.enable('de')
        dagor_motors.set_speed('ha', 0)
        dagor_motors.set_speed('de', 0)

    @staticmethod
    def adjust_speeds(ha_err, de_err, max_ha, max_de):
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

    @staticmethod
    def slope(speed, err, a=15, b=400):
        """Calculate real speed given position error."""
        return (a * speed * abs(err) ** 2 / b) ** (1 / 3)

    @staticmethod
    def speed_real_to_motor(speed, limit):
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
        """Stop tracking but keep the loop running"""
        self.config['tracking'] = False
        self.config['target_celest'] = dagor_position.get_celest()
        self._dump_config_json()

    @staticmethod
    def halt_tracking():
        """Stop tracking and halt the loop."""
        print_("Stopping")
        dagor_motors.stop()
        wait_for_stop(dagor_motors.get_motor('ha'), dots=True)
        wait_for_stop(dagor_motors.get_motor('de'), dots=True)

    @staticmethod
    def update_path(path, current_internal):
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

    def _loop_reload_config(self):
        self._load_config_json()
        self._process_config()

    def _loop_read_inputs(self):
        self.config['target_internal'] = dagor_position.celest_to_internal(
            self.target.celest(),
            self.config['chirality'],
        )
        internal = dagor_position.get_internal()
        self.current['internal'] = internal
        self.current['t_now'] = time()
        self.current['now'] = datetime.utcnow()
        self.current['offset'] = read_corrections_file()
        self.current['celest'] = dagor_position.internal_to_celest(internal)
        self.current['chirality'] = dagor_position.get_chirality(
            internal['de'])
        self.current['altaz'] = dagor_position.celest_to_altaz(
            self.current['celest'])

    def _loop_calculate_path_target(self):
        self.current['path'] = dagor_path.get_path(
            self.current['internal'],
            self.config['target_internal'],
            force=self.config['force'],
        )
        self.current['path'] = self.update_path(
            self.current['path'], self.current['internal'])
        self.path_target = self.current['path'][1]

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
        self.current['ha_err'] = ha_now - self.path_target.ha
        self.current['de_err'] = de_now - self.path_target.de

    def _loop_calculate_speeds(self):
        speeds = self.adjust_speeds(
            self.current['ha_err'],
            self.current['de_err'],
            dagor_motors.MAX_SPEED_HA,
            dagor_motors.MAX_SPEED_DE,
        )
        b = 200 if self.config['rough'] else 400
        # HA:
        ha_speed = self.slope(
            speeds['speed_ha'], self.current['ha_err'], a=20, b=b)
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

        # TODO This is ugly hardcoded gear direction switch accelerator! OMG!
        now = datetime.utcnow()
        if 0 < abs(de_speed) < 15 and de_speed == self._last_de_speed:
            if self._de_speed_same_since is None:
                self._de_speed_same_since = now
            if now - self._de_speed_same_since > timedelta(seconds=2):
                self._de_warp_speed = True
        else:
            self._de_warp_speed = False
            self._de_speed_same_since = None
        self._last_de_speed = de_speed
        if self._de_warp_speed:
            print_("!!DE!!", end="")
            de_speed *= 8
        if self.config['target_is_static']:
            if 0 < abs(ha_speed) < 15 and ha_speed == self._last_ha_speed:
                if self._ha_speed_same_since is None:
                    self._ha_speed_same_since = now
                if now - self._ha_speed_same_since > timedelta(seconds=2):
                    self._ha_warp_speed = True
            else:
                self._ha_warp_speed = False
                self._ha_speed_same_since = None
            self._last_ha_speed = ha_speed
            if self._ha_warp_speed:
                print_("!!HA!!", end="")
                sys.stdout.flush()
                ha_speed *= 8

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
            self.current['on_target'] = time_on_target > TARGET_SETTLE_WAIT
            self._slewing = False
        self.current['slewing'] = self._slewing

    def _loop_check_at_home(self):
        altaz = self.current['altaz']
        alt_diff = altaz['alt'] - dagor_position.HOME_ALTAZ['alt']
        az_diff = (altaz['az'] - dagor_position.HOME_ALTAZ['az']) \
                  * math.cos(math.radians(altaz['alt']))
        diff = (alt_diff**2 + az_diff**2)**0.5
        self.current['at_home'] = diff < AT_HOME_ZONE

    def _loop_check_at_park(self):
        altaz = self.current['altaz']
        alt_diff = altaz['alt'] - dagor_position.PARK_ALTAZ['alt']
        az_diff = (altaz['az'] - dagor_position.PARK_ALTAZ['az']) \
                  * math.cos(math.radians(altaz['alt']))
        diff = (alt_diff**2 + az_diff**2)**0.5
        self.current['at_park'] = diff < AT_PARK_ZONE

    def _loop_stop_on_target(self):
        if self.config['stop_on_target'] and self.current['on_target']:
            raise EnterAbort()

    def _loop_apply_speeds(self):
        if not self.config['tracking']:
            self.current['ha_speed'] = 0
            self.current['de_speed'] = 0
        dagor_motors.set_speed('ha', self.current['ha_speed'])
        dagor_motors.set_speed('de', self.current['de_speed'])

    def _loop_write_current(self):
        self._dump_current_json()

    def track(self):
        if not self.config:
            raise RuntimeError('tracking not configured')

        self._reset_current()
        self.init_motors()

        self.current['on_target'] = False
        self.current['on_target_since'] = None
        try:
            while True:
                try:
                    self._loop_wait()
                    self._loop_reload_config()
                    self._loop_read_inputs()
                    self._loop_calculate_path_target()
                    self._loop_check_space()
                    self._loop_calculate_errors()
                    self._loop_check_at_home()
                    self._loop_check_at_park()
                    self._loop_calculate_speeds()
                    self._loop_check_target()
                    self._loop_apply_speeds()
                    self._loop_print_console_header()
                    self._loop_print_stats()
                    self._loop_stop_on_target()
                    self._loop_write_current()

                except dagor_path.NoPath as e:
                    print_("No path: {}".format(e))
                    print_(self.target.celest())
                    self.config['tracking'] = False

                except ValidationError as e:
                    print_("Schema error: {}".format(e))
                    self.stop_tracking()

        except (EnterAbort, KeyboardInterrupt):
            # Note: KeyboardInterrupt will sometimes not get caught here
            # ... it's a thing
            print("")
            self.halt_tracking()

        except Exception:
            print_('Halting track loop!!!')
            self.halt_tracking()
            print_('Original error:')
            raise

    def _loop_print_console_header(self):
        print_("\x1b[2J\x1b[0;0H")
        print_('DAGOR GOTO')
        print_(self.current['now'])
        print_('----------------')
        print_("Press Enter to stop at any time")
        print_("")

    def _loop_print_stats(self):
        od = OrderedDict()
        od['speeds'] = '{:>14}  {:>14}'.format(
            self.current['ha_speed'], self.current['de_speed'])
        od['errors'] = '{:>14}  {:>14}'.format(
            format_degrees(self.current['ha_err'] * 15),
            format_degrees(self.current['de_err']),
        )
        od['celest'] = '{:>14}  {:>14}'.format(
            format_hours(self.current['celest']['ra']),
            format_degrees(self.current['celest']['de']),
        )
        od['offset'] = '{:>14}  {:>14}'.format(
            format_hours(self.current['offset']['ha_offset']),
            format_degrees(self.current['offset']['de_offset']),
        )
        altaz = dagor_position.get_altaz()
        od['altaz '] = '{:>14}  {:>14}'.format(
            format_degrees(altaz['alt']), format_degrees(altaz['az']))
        od['chiral'] = self.current['chirality']
        if self.config['chirality'] != self.current['chirality']:
            od['chiral'] += '  ->  {}'.format(self.config['chirality'])
        od['mode  '] = '{}, {}'.format(
            'static' if self.config['target_is_static'] else 'sky',
            'precise' if not self.config['rough'] else 'quick',
        )
        od['target'] = '{:>14}  {:>14}'.format(
            format_hours(self.config['target_celest']['ra']),
            format_degrees(self.config['target_celest']['de']),
        )
        od['inter.'] = '{:0<14}  {:0<14}'.format(
            self.current['internal']['ha'],
            self.current['internal']['de'])
        for i, point in enumerate(self.current['path'][1:], start=1):
            od['path_{}'.format(i)] = '{:0<14}  {:0<14}'.format(
                point.ha, point.de)
        od['on_target'] = self.current['on_target']
        p_(od, end="\n.")


def speed_tracking(target_celest=None,
                   chirality=None,
                   target_is_static=False,
                   stop_on_target=False,
                   rough=False,
                   force=False):
    tracking = Tracking()
    tracking.set_config({
        'target_celest': target_celest,
        'chirality': chirality,
        'target_is_static': target_is_static,
        'stop_on_target': stop_on_target,
        'rough': rough,
        'force': force,
        'tracking': True,
    })
    tracking.track()


def run():
    tracking = Tracking()
    tracking.set_config({
        'target_celest': None,
        'chirality': None,
        'target_is_static': True,
        'stop_on_target': False,
        'rough': False,
        'force': False,
        'tracking': False,
    })
    tracking.track()


def set_config(config):
    tracking = Tracking()
    tracking.set_config(config)


def get_config():
    tracking = Tracking()
    return tracking.get_config()


def get_current():
    with open(conf.TRACKING['tracking_current_file'], 'r') as f:
        content = f.read()
        try:
            current = json.loads(content)
        except ValueError:
            logger.exception('Error ')
            return None
    return current


def get_status():
    status = {
        'config': get_config(),
        'current': get_current(),
    }
    return status


# Run as CLI client


def _main(args):

    if args['start']:
        tracking = Tracking()
        tracking.track()

    if args['run']:
        run()

    if args['sync']:
        raise NotImplementedError()

    if args['sync_console']:
        sync_console()


if __name__ == '__main__':

    cli_args = docopt(
        __doc__, version=__doc__.split("\n")[0], options_first=True)

    if len(sys.argv) == 1 or cli_args['help']:
        print __doc__.strip()
        exit(0)

    _main(cli_args)
