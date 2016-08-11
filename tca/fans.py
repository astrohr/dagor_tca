#!/usr/bin/env python
"""Dagor switches interface version 0.8.

Usage:
    fans.py status
    fans.py set <FAN_ID> <FAN_STATE>
    fans.py get <FAN_ID>
    fans.py -h | --help
    fans.py --version

Commands:
    status         Display fan switches states
    set            Set fan switches ("0" -> Central, "1" -> Other fans) to desired state:
                   "0" -> OFF
                   "1" -> Speed 1
                   "2" -> Speed 2

Options:
    -h --help      Show this screen or description of specific command.
    --version      Show version.
"""
from __future__ import print_function, division, absolute_import

import sys

from docopt import docopt
import tca.switches_analogue as dagor_analogue_switches
from tca.local import fans_config


CommunicationException = dagor_analogue_switches.CommunicationException


class FansController(dagor_analogue_switches.AnalogueSwitchController):
    PORT = fans_config.PORT
    RESET_DISABLED = True

    def __init__(self):
        super(FansController, self).__init__()  # Just keeping PEP8 happy...

    instance = None
    
    @classmethod
    def get_instance(cls):
        if cls.instance is None:
            cls.instance = cls()
        return cls.instance
    

def get_controller():
    return FansController.get_instance()


def get_fan(n):
    controller = get_controller()
    status = controller.status_analogue
    return int(status[n - 1])


def get_fans():
    controller = get_controller()
    status = controller.status_analogue
    return status


def set_fan(fan_id, state):
    controller = get_controller()
    controller.switch_analogue(fan_id, state)


def _main(args):
    if args['set']:
        fan_id = int(args['<FAN_ID>'])
        state = int(args['<FAN_STATE>'])
        set_fan(fan_id, state)

    if args['get']:
        fan_id = int(args['<FAN_ID>'])
        print(get_fan(fan_id))

    elif args['status']:
        status = get_fans()
        print("Central Fan: " + status[0])
        print("Other Fans: " + status[1])

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
