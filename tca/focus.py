#!/usr/bin/env python
"""Dagor focus interface version 0.8.

Usage:
    focus.py status
    focus.py step to <N>
    focus.py step by <N>
    focus.py stop
    focus.py set <N>
    focus.py get
    focus.py -h | --help
    focus.py --version

Commands:
    status          Display status
    step to         Move to specified position
    step by         Move by specified number of steps
    stop            Stop the motor
    set             Set current position
    get             Get current position, to stdout

Options:
    -h --help       Show this screen or description of specific command.
    --version       Show version.
"""
from __future__ import print_function, division, absolute_import
from common import fix_path, EnterAbort, _wait_for_time, print_

fix_path(__name__)

from docopt import docopt
import serial
import sys
import time

from _utils import str_bool
from logging_conf import get_logger
from tca.common import exit_

logger = get_logger(__file__)


RESET_DISABLED = True

SERIAL = {
    'PORT': '/dev/ttyUSB0',  # TCS
    # 'PORT': '/dev/cu.wchusbserialfa130',  # Mac
    'BAUDRATE': 115200,
    'TIMEOUT': 0.1,  # if no data available, block for max this many seconds
}

RETRIES = 5
INTERVAL = 1  # seconds


class FocuserController(object):
    """ Controls the focuser, responds to serial/USB communication.

        Note: it always requests latest status from Arduino controller.
        
     .. Serial settings: 
        115200 baud, newline line ending

     .. Protocol:
         TODO describe protocol
    """

    # public API
    @property
    def position(self):
        self._refresh_status()
        return self._status['position']

    @property
    def can_go_up(self):
        self._refresh_status()
        return self._status['can_go_up']

    @property
    def status(self):
        self._refresh_status()
        return self._status

    def __init__(self, SERIAL=SERIAL, RESET_DISABLED=RESET_DISABLED):
        """ Open and confirm communication, request current status """
        self._SERIAL = SERIAL
        self._RESET_DISABLED = RESET_DISABLED
        self._open_serial()

    def pretty_status(self):
        self._refresh_status()
        for key, val in self._status.iteritems():
            print('{}: {}'.format(key, val))

    def step_to(self, n):
        try:
            retry = RETRIES
            while retry:
                retry -= 1
                try:
                    self._step_to(n)
                except StateException:
                    logger.info("not idle, stopping")
                    self._stop()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException('Failed "step to" after {} retries'
                                             .format(RETRIES))
            on_target = False
            print_("positioning.", end='')
            while not on_target:
                _wait_for_time(INTERVAL, dots=True,
                               enter_abort=True,
                               end='')
                self._refresh_status()
                on_target = self._status['idle']
            print()
            print('position: {}'.format(self._status['position']))
        except (KeyboardInterrupt, EnterAbort):
            self.stop()

    def step_by(self, n):
        try:
            retry = RETRIES
            while retry:
                retry -= 1
                try:
                    self._step_by(n)
                except StateException:
                    logger.info("not idle, stopping")
                    self._stop()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException('Failed "step by" after {} retries'
                                             .format(RETRIES))
            on_target = False
            print_("positioning.", end='')
            while not on_target:
                _wait_for_time(INTERVAL, dots=True,
                               enter_abort=True,
                               end='')
                self._refresh_status()
                on_target = self._status['idle']
            print()
            print('position: {}'.format(self._status['position']))
        except (KeyboardInterrupt, EnterAbort):
            self.stop()

    def stop(self):
        print_('stopping.', end='')
        self._stop()
        self._refresh_status()
        idle = self._status['idle']
        while not idle:
            _wait_for_time(INTERVAL, dots=True,
                           enter_abort=False,
                           end='')
            self._refresh_status()
            idle = self._status['idle']
        print()
        print('stopped at: {}'.format(self._status['position']))

    def set(self, n):
        self._set(n)

    def get(self):
        print(self._get())

    # private members

    _status = None
    _default_status = {
        'position': 0,
        'idle': False,
        'stopping_hard': False,
        'moving_up': False,
        'moving_dn': False,
        'intent_up': False,
        'intent_dn': False,
        'can_go_up': False,
        'can_go_dn': False,
    }
    _default_status_type = {
        'position': int,
        'idle': str_bool,
        'stopping_hard': str_bool,
        'moving_up': str_bool,
        'moving_dn': str_bool,
        'intent_up': str_bool,
        'intent_dn': str_bool,
        'can_go_up': str_bool,
        'can_go_dn': str_bool,
    }
    _serial = None  # serial.Serial()

    def _open_serial(self):
        """ Open serial port to Arduino controller. Waits for acknowledgement (string "ready").
            Throws CommunicationException.
        """
        self._serial = serial.Serial(
            port=self._SERIAL['PORT'],
            baudrate=self._SERIAL['BAUDRATE'],
            timeout=self._SERIAL['TIMEOUT'],
            dsrdtr=not RESET_DISABLED,
            rtscts=not RESET_DISABLED,
        )
        # self._serial.open()
        if not self._RESET_DISABLED:
            for i in range(60):  # TODO magic magic magic!
                line = self._serial.readline().strip()
                logger.debug("line: {}".format(line))
                if line and line == 'ready':
                    pass
                    #return  # TODO maybe wait for next readline to timeout, so we are sure this is the last data in serial pipe?
            raise CommunicationException('Cannot establish serial communication: Arduino not reporting ready')
        else:  # self._RESET_DISABLED
            pass  # TODD send a "ready?" query?

    def _refresh_status(self):
        # send command:
        self._serial.write('status\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "status 9\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('status '):
            raise CommunicationException('Controller doesnt acknowledge "status" command, instead got: {}'.format(response))
        # parse N (number of lines that follow):
        try:
            self._n = int(response[len('status '):])
        except ValueError:
            raise CommunicationException('Expected int, got: {}'.format(response))
        # read remaining reaponse lines:
        new_status = {}
        for _ in range(self._n):
            response = self._serial.readline().strip()
            key, _, value = map(str.strip, response.partition(':'))
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

    def _step_to(self, n):
        self._refresh_status()
        if not self._status['idle']:
            logger.debug("not idle")
            raise StateException('Motor not idle')
        # send command:
        self._serial.write('step to {}\n'.format(n))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "step to" command, instead got: {}'.format(response))

    def _step_by(self, n):
        self._refresh_status()
        if not self._status['idle']:
            logger.debug("not idle")
            raise StateException('Motor not idle')
        # send command:
        self._serial.write('step by {}\n'.format(n))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "step by" command, instead got: {}'.format(response))

    def _stop(self):
        logger.debug("stop")
        # send command:
        self._serial.write('stop\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "stopping 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('stopping 0'):
            raise CommunicationException('Controller doesnt acknowledge "stop" command, instead got: {}'.format(response))

    def _set(self, n):
        # send command:
        self._serial.write('position set {}\n'.format(n))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 1\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 1'):
            raise CommunicationException('Controller doesnt acknowledge "set" command, instead got: {}'.format(response))
        response = self._serial.readline().strip()  # expect: new position number
        if not response.startswith('{}'.format(n)):
            raise CommunicationException('Controller acknowledges "set" command, but returned wrong position: {}'.format(response))

    def _get(self):
        # send command:
        self._serial.write('position\n'.format())
        # read first response line:
        response = self._serial.readline().strip()  # expect: "position 1\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('position 1'):
            raise CommunicationException(
                'Controller doesnt acknowledge "get" command, instead got: {}'.format(
                    response))
        response = self._serial.readline().strip()  # expect: position number
        try:
            n = int(response)
        except ValueError:
            raise CommunicationException(
                'Controller acknowledges "get" command, but returned something strange: {}'.format(
                    response))
        return n


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass


class StateException(Exception):
    """ Cannot complete the request at this time """
    pass


def _main(args):
    controller = FocuserController(SERIAL, RESET_DISABLED)

    if args['status']:
        controller.pretty_status()
    if args['stop']:
        controller.stop()
    elif args['step'] and args['to']:
        n = int(args['<N>'])
        controller.step_to(n)
    elif args['step'] and args['by']:
        n = int(args['<N>'])
        controller.step_by(n)
    elif args['set']:
        n = int(args['<N>'])
        controller.set(n)
    elif args['get']:
        controller.get()

    exit(0)


if __name__ == '__main__':
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
