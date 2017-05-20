#!/usr/bin/env python
# coding=utf-8
"""Dagor dome rotation interface version 1.3.

Usage:
    dome.py server
    dome.py status
    dome.py calibrate
    dome.py goto <azimuth>
    dome.py azimuth set <azimuth>
    dome.py azimuth get
    dome.py park
    dome.py door (open|close|stop)
    dome.py home (set|get)
    dome.py (up|down|stop)
    dome.py -h | --help
    dome.py --version

Commands:
    server         Start Flusk server.
    status         Dome status (azimuth \\n rotating or not \\n calibrated, 
    during calibration or uncalibrated).
    calibrate      Start calibration.
    goto           Move dome to specified azimuth.
    azimuth set    Set specified azimuth as current position.
    azimuth get    Get current azimuth.
    park           Move dome to home azimuth.
    door           Open or close door.
    home set       Set home to current azimuth.
    home get       Get home azimuth.
    up             Start rotation of the dome (south-east-north-west).
    down           Start rotation of the dome (south-wast-north-east).
    stop           Stop rotation of the dome.

Options:
    -h --help      Show this screen.
    --version      Show version.
"""

from  __future__ import print_function, division, absolute_import
from _controller import (
    CommunicationException,
    StateException,
    BaseController,
    CliMixin,
)
from common import EnterAbort, _wait_for_time, print_, exit_
from local.configuration import DOME_CONFIG

from docopt import docopt
import sys
import time

from logging_conf import get_logger

logger = get_logger(__file__)


class DomeController(CliMixin, BaseController):
    """ 
    Controls the dome rotation and doors, responds to serial/USB 
    communication.

    Note: it always requests latest status from Arduino controller.

    .. Serial settings: 
        9600 baud, newline line ending

    .. Protocol:
         TODO describe protocol
    """

    ROTATION_IDLE = 0
    ROTATION_UP = -1
    ROTATION_DOWN = 1

    CALIBRATION_NONE = -1
    CALIBRATION_DONE = 1
    CALIBRATION_IN_PROGRESS = 0

    UNKNOWN_AZIMUTH = 500.0

    # public API

    @property
    def idle(self):
        self._refresh_status()
        return self._status['rotation'] == self.ROTATION_IDLE

    @property
    def azimuth(self):
        self._refresh_status()
        return self._status['azimuth']

    def door_open(self):
        """CLI handler with retries"""
        self._simple_cli_handler(self._door_open)
        print_("door opening.")
        # TODO sense door closed, and implement dots while closing

    def door_close(self):
        """CLI handler with retries"""
        self._simple_cli_handler(self._door_close)
        print_("door closing.")
        # TODO sense door closed, and implement dots while closing

    def door_stop(self):
        """CLI handler with retries"""
        self._simple_cli_handler(self._door_stop)
        print_("door stopped.")
        # TODO sense door closed, and implement dots while closing

    def rotate_up(self):
        """
        CLI function with dots for rotating the dome "up".
        """
        # TODO verify that the dome is actually moving, abort if it stops

        print_('dome moving "up"', end='')
        self._dots_cli_handler(self._rotate_up, self._rotate_stop)

    def rotate_down(self):
        """
        CLI function with dots for rotating the dome "down".
        """
        # TODO verify that the dome is actually moving, abort if it stops
        print_('dome moving "down"', end='')
        self._dots_cli_handler(self._rotate_down, self._rotate_stop)

    def rotate_stop(self):
        """
        CLI function to stop dome rotation".
        """
        print_('dome stopping.')
        self._rotate_stop()

    # private members

    _default_status = {
        'rotation': ROTATION_IDLE,
        'azimuth': 0,
        'calibration': CALIBRATION_NONE,
    }
    _default_status_type = {
        'rotation': int,
        'azimuth': float,
        'calibration': int,
    }

    def prettify_val(self, key, val):
        """Make statuses human-readable"""
        if key == 'rotation':
            return {
                self.ROTATION_IDLE: 'idle',
                self.ROTATION_UP: 'up',
                self.ROTATION_DOWN: 'down',
            }[val]
        if key == 'calibration':
            return {
                self.CALIBRATION_NONE: 'none',
                self.CALIBRATION_DONE: 'done',
                self.CALIBRATION_IN_PROGRESS: 'in_progress',
            }[val]
        if key == 'azimuth':
            if val == self.UNKNOWN_AZIMUTH:
                return 'unknown'
        return val

    def _refresh_status(self):
        """
        Implementation specific to Dome, because protocol 
        is not fully "common".
        """
        data = self._simple_command('status')
        new_status = {}
        keys = (k for k in
                ('azimuth', 'rotation', 'calibration', 'unknown_key'))
        for value in data:
            key = next(keys)
            if key not in self._default_status:
                raise CommunicationException(
                    'Got unknown status key: {}: {}'.format(key, value))
            new_status[key] = self._default_status_type[key](value)
        if len(new_status) != len(self._default_status):
            raise CommunicationException(
                'Mismatched status keys:\n'
                'new_status:\n'
                '{}\n'
                'known keys:\n'
                '{}'
                .format(new_status.keys(), self._default_status.keys()))
        self._status = new_status

    def _door_open(self):
        self._simple_command('door_open')


    def _door_close(self):
        self._simple_command('door_close')


    def _door_stop(self):
        self._simple_command('door_stop')


    def _rotate_up(self):
        self._simple_command('up')


    def _rotate_down(self):
        self._simple_command('down')


    def _rotate_stop(self):
        self._simple_command('stop')


def get_controller():
    """
    Get controller singleton
    :return: DomeController
    """
    return DomeController.get_instance(DOME_CONFIG)


def get_status():
    """
    :return: dict
    """
    return get_controller().status


def get_idle():
    """
    :return: boolean 
    """
    return get_controller().idle


def dome_up():
 get_controller().rotate_up()


def dome_down():
    get_controller().rotate_down()


def dome_stop():
    controller = get_controller()
    assert isinstance(controller, DomeController)
    controller.rotate_stop()
    controller.door_stop()


def dome_open():
    return get_controller().door_open()


def dome_close():
    return get_controller().door_close()


def _main(args):
    """CLI main branching"""

    controller = get_controller()
    assert isinstance(controller, DomeController)

    if args['status']:
        controller._refresh_status()
        controller.pretty_status()
        exit(0)

    if args['door']:

        if args['open']:
            controller.door_open()
            exit(0)

        if args['stop']:
            controller.door_stop()
            exit(0)

        if args['close']:
            controller.door_close()
            exit(0)

    if args['up']:
        controller.rotate_up()
        exit(0)

    if args['down']:
        controller.rotate_down()
        exit(0)

    if args['stop']:
        controller._rotate_stop()
        exit(0)

    elif args['calibrate']:
        raise NotImplementedError()
    elif args['goto']:
        raise NotImplementedError()
    elif args['azimuth']:
        if args['set']:
            raise NotImplementedError()
        elif args['get']:
            raise NotImplementedError()
    elif args['park']:
        raise NotImplementedError()
    elif args['home']:
        if args['set']:
            raise NotImplementedError()
        elif args['get']:
            raise NotImplementedError()


if __name__ == '__main__':
    # CLI entry point
    args = docopt(__doc__, version=__doc__.split('\n'[0]), options_first=True)
    if len(sys.argv) == 1:
        print(__doc__.strip())
        exit(0)

    try:
        _main(args)
    except Exception as e:
        logger.warning(e, exc_info=True)
        # raise
        exit_('ERROR')
