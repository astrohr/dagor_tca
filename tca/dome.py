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
from _controller import CommunicationException, StateException, \
    simple_command, \
    BaseController, simple_cli_handler
from common import EnterAbort, _wait_for_time, print_, exit_
from logging_conf import get_logger
from local import dome_config

from docopt import docopt
import sys
import time

logger = get_logger(__file__)

RESET_DISABLED = True

SERIAL = {
    'port': dome_config.PORT,
    'baud_rate': 9600,
    'timeout': 1,  # if no data available, block for max this many seconds
    'retries': 5,
    'interval': 1,  # seconds
    'reset_disabled': True,
}


class DomeController(BaseController):
    """ Controls the dome rotation and doors, responds to serial/USB 
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

    # public API

    @property
    def idle(self):
        self._refresh_status()
        return self._status['rotation'] == self.ROTATION_IDLE

    @property
    def azimuth(self):
        self._refresh_status()
        return self._status['azimuth']

    @simple_cli_handler('_door_open')
    def door_open(self, method_return_value):
        """CLI handler with retries"""
        print_("door opening.")
        # TODO sense door closed, and implement dots while closing

    @simple_cli_handler('_door_close')
    def door_close(self, method_return_value):
        """CLI handler with retries"""
        print_("door closing.")
        # TODO sense door closed, and implement dots while closing

    @simple_cli_handler('_door_stop')
    def door_stop(self, method_return_value):
        """CLI handler with retries"""
        print_("door stopped.")
        # TODO sense door closed, and implement dots while closing

    #@cli_handler_with_top(start='_rotate_up', stop='_rotate_stop')
    def rotate_up(self):
        """
        CLI function with dots for rotating the dome "up".
        """
        # TODO abstract and extract CLI functions from hardware controller
        try:
            retry = self._retries
            while retry:
                retry -= 1
                try:
                    self._rotate_up()
                except StateException:
                    logger.info("not idle, stopping")
                    self._rotate_stop()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException(
                    'Failed "rotate_up" after {} retries'
                        .format(self._retries)
                )
            print_('dome moving "up"', end='')
            while True:
                _wait_for_time(self._interval, dots=True,
                               enter_abort=True,
                               end='')
        except (KeyboardInterrupt, EnterAbort):
            print('stopping')
            self._rotate_stop()

    def rotate_down(self):
        """
        CLI function with dots for rotating the dome "down".
        """
        # TODO abstract and extract CLI functions from hardware controller
        try:
            retry = self._retries
            while retry:
                retry -= 1
                try:
                    self._rotate_down()
                except StateException:
                    logger.info("not idle, stopping")
                    self._rotate_stop()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException(
                    'Failed "rotate_down" after {} retries'
                        .format(self._retries)
                )
            print_('dome moving "down"', end='')
            while True:
                _wait_for_time(self._interval, dots=True,
                               enter_abort=True,
                               end='')
        except (KeyboardInterrupt, EnterAbort):
            print('stopping')
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

    def _refresh_status(self):
        """
        Implementation specific to Dome, because protocol 
        is not fully "common".
        """
        # send command:
        self._serial.write('status\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "status 3\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok '):
            if response.startswith('error '):
                try:
                    n = int(response[len('error '):])
                    for _ in range(n):
                        additional_response = self._serial.readline().strip()
                        response = "{}\n{}".format(response,
                                                   additional_response)
                except:
                    pass
            raise CommunicationException(
                'Controller doesnt acknowledge "status" command, instead '
                'got: "{}"'.format(
                    response))
        # parse N (number of lines that follow):
        try:
            self._n = int(response[len('ok '):])
        except ValueError:
            raise CommunicationException(
                'Expected int, got: "{}"'.format(response))
        # read remaining response lines:
        new_status = {}
        keys = (k for k in
                ('azimuth', 'rotation', 'calibration', 'unknown_key'))
        for _ in range(self._n):
            response = self._serial.readline().strip()
            key = next(keys)
            value = response.strip()
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

    @simple_command('door_open')
    def _door_open(self, response_data):
        pass

    @simple_command('door_close')
    def _door_close(self, response_data):
        pass

    @simple_command('door_stop')
    def _door_stop(self, response_data):
        pass

    @simple_command('up')
    def _rotate_up(self, response_data):
        pass

    @simple_command('down')
    def _rotate_down(self, response_data):
        pass

    @simple_command('stop')
    def _rotate_stop(self, response_data):
        pass


def get_controller():
    """
    Get controller singleton
    :return: DomeController
    """
    return DomeController.get_instance(SERIAL)


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


def _main(args):
    """CLI main branching"""

    controller = get_controller()
    assert isinstance(controller, DomeController)

    if args['status']:
        print("MAIN STATUS")
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
