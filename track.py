#!/usr/bin/env python
"""Dagor tracking 1.0

Usage:
  track.py start
  track.py speed
  track.py sp
  track.py sync
  motors.py [-h | --help | help]
  motors.py --version

Commands:
  start             Start dynamic tracking.
  speed             Set approximate tracking speed.
  sync              Start interactive mode - allows to adjust position and sync celest coordinates

Options:
  -h --help         Show this screen.
  --quiet           Only data on stdout


"""
from __future__ import division
from collections import OrderedDict
from docopt import docopt
from time import time, sleep
import os
import sys
from common import NonBlockingConsole, BASE_PATH, EnterAbort, print_, _wait_for_time, sign, p_, _wait_for_stop
from formats import format_hours, format_degrees, parse_degrees, parse_hours
import motors as dagor_motors
import position as dagor_position

TRACKING_COORDINATES_FILE = os.path.join(BASE_PATH, 'coords.txt')
TRACKING_CORRECTIONS_FILE = os.path.join(BASE_PATH, 'tracking_corrections.txt')
TRACKING_CORRECTIONS_STEP_HA = parse_degrees('0:01')
TRACKING_CORRECTIONS_STEP_DE = parse_hours('0h01m')


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
            return dagor_position.celest_to_internal(celest)
    except (IOError, ValueError):
        return None


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


def adjust_speed(ha_err, de_err, max_ha, max_de, case):
    t_ha = abs(ha_err) / max_ha
    t_de = abs(de_err) / max_de
    t = max(t_ha, t_de)
    max_ha = min(t_ha / t * max_ha, max_ha)
    max_de = min(t_de / t * max_de, max_de)
    if case==1:
        return max_ha
    else:
        return max_de


def sync_console():
    with NonBlockingConsole() as nbc:
        print "\x1b[2J"
        print "\x1b[0;0HThis is tracking sync console"
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
            print "CTRL + ARROWS : Big nudge"
            print "SHIFT + S : Synchronize"
            print "ESCAPE : quit"
            print
            #print data.encode('string-escape') if data and data != '\x1b' else last_data.encode('string-escape')
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
                '\x1b[1;5A': 'CTRL_ARROW_UP',
                '\x1b[1;5B': 'CTRL_ARROW_DOWN',
                '\x1b[1;5C': 'CTRL_ARROW_LEFT',
                '\x1b[1;5D': 'CTRL_ARROW_RIGHT',
                'S': 'SYNC',
            }
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
                manual_corrections['ha_offset'] += TRACKING_CORRECTIONS_STEP_HA / 6
            elif known_sequences[data] == 'SHIFT_ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACKING_CORRECTIONS_STEP_HA / 6
            elif known_sequences[data] == 'CTRL_ARROW_UP':
                manual_corrections['de_offset'] -= TRACKING_CORRECTIONS_STEP_DE * 10
            elif known_sequences[data] == 'CTRL_ARROW_DOWN':
                manual_corrections['de_offset'] += TRACKING_CORRECTIONS_STEP_DE * 10
            elif known_sequences[data] == 'CTRL_ARROW_RIGHT':
                manual_corrections['ha_offset'] += TRACKING_CORRECTIONS_STEP_HA * 10
            elif known_sequences[data] == 'CTRL_ARROW_LEFT':
                manual_corrections['ha_offset'] -= TRACKING_CORRECTIONS_STEP_HA * 10
            if 'ARROW' in known_sequences[data]:  # any arrow
                if abs(manual_corrections['ha_offset']) < TRACKING_CORRECTIONS_STEP_HA / 6:
                    manual_corrections['ha_offset'] = 0
                if abs(manual_corrections['de_offset']) < TRACKING_CORRECTIONS_STEP_DE / 6:
                    manual_corrections['de_offset'] = 0
                write_correction_file(manual_corrections)

            elif known_sequences[data] == 'SYNC':
                reset_correction_file()
                internal = dagor_position.get_internal()
                new_internal = {
                    'ha': internal['ha'] + manual_corrections['ha_offset'],
                    'de': internal['de'] + manual_corrections['de_offset'],
                }
                dagor_position.set_internal(new_internal)
                manual_corrections['ha_offset'] = 0
                manual_corrections['de_offset'] = 0
                write_correction_file(manual_corrections)


def speed_tracking(manual_internal=None):

    print_('DYNAMIC TRACKING')
    print_('----------------')
    dagor_motors.init()
    if dagor_motors._ha.OperationMode != dagor_motors.OP_MODE_SPEED:
        dagor_motors.require_stopped(dagor_motors._ha)
    dagor_motors.require_stopped(dagor_motors._de)
    dagor_motors._ha.enable()
    dagor_motors._de.enable()
    dagor_motors._ha.OperationMode = dagor_motors.OP_MODE_SPEED
    dagor_motors._de.OperationMode = dagor_motors.OP_MODE_SPEED
    dagor_motors._de.SetSpeed = 0
    print_("Press Enter to stop at any time")
    # start:

    print_("Tracking")


    t_start = time()  #@TODO make sure this behaves well on leap seconds


    start_internal = dagor_position.get_internal()


    try:
        while True:
            _wait_for_time(dagor_motors._TRACKING_CHECK_INTERVAL, dots=True, enter_abort=True, interval=dagor_motors._TRACKING_CHECK_INTERVAL / 10, dot_skip=10)
            manual_corrections = read_corrections_file()


            manual_coords = read_coordinates_file()

            if manual_internal:
                internal = manual_internal
                start_internal = dagor_position.get_internal()
                t_start = time()
            elif manual_coords is not None:
                internal = read_coordinates_file()
                start_internal = dagor_position.get_internal()
                t_start = time()
            else:
                internal = start_internal

            ha_target, de_target = internal['ha'], internal['de']


            t_now = time()
            internal_now = dagor_position.get_internal()
            ha_now, de_now = internal_now['ha'] + manual_corrections['ha_offset'], internal_now['de'] + manual_corrections['de_offset']

            ha_err = (ha_now - ha_target) - (t_now - t_start) / 60 / 60 / 0.99726958  # sidereal day

            de_err = (de_now - de_target)

            ha_speed = (9 * adjust_speed(ha_err, de_err, dagor_motors.MAX_SPEED_HA, dagor_motors.MAX_SPEED_DE, 1) * abs(ha_err) ** 2 / 500) ** (1 / 3)
            ha_speed = ha_speed * dagor_motors.SPEED_LIMIT / dagor_motors.MAX_SPEED_HA
            ha_speed = min( ha_speed - (27 if sign(ha_err) == 1 else -27), dagor_motors.SPEED_LIMIT )
            ha_speed = int( ha_speed * sign(ha_err) )
            
            de_speed = (9 * adjust_speed(ha_err, de_err, dagor_motors.MAX_SPEED_HA, dagor_motors.MAX_SPEED_DE, 2) * abs(de_err) ** 2 / 500) ** (1 / 3)
            de_speed = de_speed * dagor_motors.SPEED_LIMIT / dagor_motors.MAX_SPEED_DE
            de_speed = min( de_speed, dagor_motors.SPEED_LIMIT )
            de_speed = int( de_speed * sign(de_err) )

            od = OrderedDict()
            od['ha_speed'] = ha_speed
            od['ha_err'] = ha_err
            od['de_speed'] = de_speed
            od['de_err'] = de_err
            celest = dagor_position.get_celest()
            od['ra'] = format_hours(celest['ra'])
            od['de'] = format_degrees(celest['de'])
            od['manual_coords'] = manual_coords
            p_(od)

            dagor_motors._ha.SetSpeed = ha_speed
            dagor_motors._de.SetSpeed = de_speed

    except EnterAbort:
        print_("Stopping")
        dagor_motors._ha.SetSpeed = 0
        dagor_motors._de.SetSpeed = 0
        _wait_for_stop(dagor_motors._ha, dots=True)
        _wait_for_stop(dagor_motors._de, dots=True)
        print_("END")


# Run as CLI client


def _main(args):

    if args['start']:
        reset_correction_file()
        dagor_motors.init()
        speed_tracking()

    if args['speed']:
        dagor_motors.init()
        dagor_motors.set_speed(dagor_motors._ha, dagor_motors.TRACKING_SPEED)

    if args['sp']:
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

