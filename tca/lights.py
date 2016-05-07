#!/usr/bin/env python
"""Dagor switches interface version 0.8.

Usage:
    lights.py status
    lights.py set (0 | 1 | 2 | 3)
    lights.py -h | --help
    lights.py --version

Commands:
    status         Display lights state, 0 to 3
    set            Switch light switches on or off. "0" means off, "3" means "all on"

Options:
    -h --help      Show this screen or description of specific command.
    --version      Show version.
"""

import sys
from docopt import docopt
import switches as dagor_switches


def get_light(n):
    controller = dagor_switches.SwitchController()
    status = controller.status
    return int(status[n])


def get_lights():
    return get_light(0) * 2 + get_light(1) * 1


def set_lights(n):
    controller = dagor_switches.SwitchController()
    if n == 0:
        controller.switch(1, False)
        controller.switch(2, False)
    elif n == 1:
        controller.switch(1, False)
        controller.switch(2, True)
    elif n == 2:
        controller.switch(1, True)
        controller.switch(2, False)
    elif n == 3:
        controller.switch(1, True)
        controller.switch(2, True)


def _main(args):

    if args['set']:
        n = None
        if args['0']:
            n = 0
        elif args['1']:
            n = 1
        elif args['2']:
            n = 2
        elif args['3']:
            n = 3
        print set_lights(n)

    elif args['status']:
        status = get_lights()
        print(status)



    exit(0)

if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.split('\n'[0]), options_first=True)
    if len(sys.argv) == 1:
        print __doc__.strip()
        exit(0)

    try:
        _main(args)
    except:
        raise
        #exit_('ERROR')