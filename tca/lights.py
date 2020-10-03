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
from __future__ import print_function, division, absolute_import

import sys
from docopt import docopt
import tca.switches as dagor_switches
from tca.local.configuration import LIGHTS_CONFIG


CommunicationException = dagor_switches.CommunicationException


class LightsController(dagor_switches.SwitchController):
    PORT = LIGHTS_CONFIG['port']
    RESET_DISABLED = LIGHTS_CONFIG['reset_disabled']


def get_light(n):
    controller = LightsController()
    status = controller.status
    return int(status[n])


def get_lights():
    controller = LightsController()
    status = controller.status
    return status[0] * 2 + status[1] * 1


def set_light(light_i, state):
    controller = LightsController()
    controller.switch(light_i, state)


def set_lights(n):
    controller = LightsController()
    if n == 0:
        controller.switch(1, False)
        controller.switch(2, False)
    elif n == 1:
        controller.switch(1, True)
        controller.switch(2, False)
    elif n == 2:
        controller.switch(1, False)
        controller.switch(2, True)
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
        print(set_lights(n))

    elif args['status']:
        status = get_lights()
        print(status)

    exit(0)


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.split('\n'[0]), options_first=True)
    if len(sys.argv) == 1:
        print(__doc__.strip())
        exit(0)

    try:
        _main(args)
    except:
        raise
        #exit_('ERROR')
