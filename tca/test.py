#!/usr/bin/env python
"""Dagor tests 0.2

Usage:
  test.py (ha | de) transmission from <angle_0> to <angle_1> speed <S> increment <I>
  test.py [-h | --help | help]
  test.py --version

Commands:
  transmission      Test motor pulse to IK220 encoder ratio.


Options:
  -h --help         Show this screen.
  --quiet           Only data on stdout
  --space=<file>    File defining allowed space region [default: space.txt]


"""
from __future__ import division
from time import sleep
from docopt import docopt
import sys
import math
from common import exit_, wait_for_stop, EnterAbort

import position


def _init():
    pass


def _transmition(axes, start, finish, speed, increment):
    import motors
    motors.init()
    if axes == 'ha':
        motor = motors._ha
    elif axes == 'de':
        motor = motors._de
    else:
        raise ValueError('Invalid axes!')
    current = position.get_internal()[axes]
    reverse = start > finish
    gear_deadzone = -500000
    increment = math.fabs(increment)
    if reverse:
        increment *= -1
        gear_deadzone *= -1
    delta = start - current
    motor.Task1_Speed = 3000
    sys.stdout.write("TRANSMISSION TEST\n")
    sys.stdout.write("Press Enter to abort\n")
    sys.stdout.write("moving to start position")
    sys.stdout.flush()
    motor.move_delta(delta, case=axes)
    wait_for_stop(motor, dots=True, after=1, skip_dots=5, enter_abort=True)
    sys.stdout.write("\naligning gears")
    sys.stdout.flush()
    motor.move_delta(-gear_deadzone)
    wait_for_stop(motor, dots=True, after=1, skip_dots=5, enter_abort=True)
    motor.move_delta(gear_deadzone)
    wait_for_stop(motor, dots=True, after=1, skip_dots=5, enter_abort=True)
    sys.stdout.write("\ntesting!\n")
    sys.stdout.write("tested_axes\t{}\n".format(axes))
    sys.stdout.write("increment\t{}\n".format(increment))
    sys.stdout.write("testing_speed\t{}\n".format(speed))
    sys.stdout.write("mean_angle\tratio\n")
    sys.stdout.flush()
    previous_angle = position.get_internal()[axes]
    previous_raw = position.get_normalised()[axes]
    motor.Task1_Speed = speed
    while True:
        motor.move_delta(increment, case=axes)
        wait_for_stop(motor, after=1, enter_abort=True)

        current_angle = position.get_internal()[axes]
        current_raw = position.get_normalised()[axes]
        pulses = motor.Position

        ratio = pulses / (current_raw - previous_raw)
        mean_angle = (previous_angle + current_angle) / 2
        sys.stdout.write("%s\t%s\n" % (mean_angle, ratio))
        sys.stdout.flush()

        if (current_angle > finish) != reverse:
            break

        sleep(0.2)
        previous_angle = current_angle
        previous_raw = current_raw


# Run as CLI client


def _main(args):
    _init()

    if args['transmission']:
        if args['ha']:
            _transmition('ha', float(args['<angle_0>']), float(args['<angle_1>']), int(args['<S>']), float(args['<I>']))
        if args['de']:
            _transmition('de', float(args['<angle_0>']), float(args['<angle_1>']), int(args['<S>']), float(args['<I>']))


if __name__ == '__main__':

    args = docopt(__doc__, version=__doc__.split("\n")[0], options_first=True)

    if len(sys.argv) == 1 or args['help']:
        print __doc__.strip()
        exit(0)

    try:
        _main(args)
        exit_('OK')
    except EnterAbort:
        exit_('ENTER_ABORT')
    except:
        raise
        #exit_('ERROR')
