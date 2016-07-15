#!/usr/bin/env python
"""Dagor focus interface version 0.8.

Usage:
    focus.py status
    focus.py step to <N>
    focus.py step by <N>
    focus.py set <N>
    focus.py -h | --help
    focus.py --version

Commands:
    status          Display status
    step to         Move to specified position
    step by         Move by specified number of steps
    set             Set current position

Options:
    -h --help       Show this screen or description of specific command.
    --version       Show version.
"""
from pprint import pprint

from docopt import docopt
import serial
import sys

from _utils import str_bool


RESET_DISABLED = False

SERIAL = {
    # 'PORT': '/dev/tty.usbserial-A703F1A4',  # Not!
    'PORT': '/dev/cu.wchusbserialfa130',  # Mac
    'BAUDRATE': 115200,
    'TIMEOUT': 0.1,  # if no data available, block for max this many seconds
}


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
        self._refresh_status()

    def pretty_status(self):
        self._refresh_status()
        pprint(self._status)

    def switch(self, n, state):
        if n == 'all':
            for i in range(0, self._n):
                self._switch(i + 1, state)
        else:
            self._switch(int(n), state)

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
                print("line: {}".format(line))
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
        if response.startswith('status '):
            raise CommunicationException('Controller doesnt acknowledge status command, instead got: {}'.format(response))
        # parse N (number of lines that follow):
        try:
            print("response:")
            print(response)
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

    def _switch(self, n, state):
        on_off = 'on' if state else 'off'
        self._serial.write('switch {} {}\n'.format(n, on_off))
        response = self._serial.readline().strip()
        if response != 'switch {}'.format(n):
            raise CommunicationException('Controller doesnt acknowlege switch command, instead got: {}'.format(response))
        response = self._serial.readline().strip()
        if response not in ('ON', 'OFF'):
            raise CommunicationException('Expected "ON" or "OFF", got: {}'.format(response))
        self._status[n - 1] = True if response == 'ON' else False


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass


def _main(args):

    controller = FocuserController(SERIAL, RESET_DISABLED)

    if args['status']:
        controller.pretty_status()        
    elif args['switch']:
        state = (args['on'] == True)
        controller.switch(args['<N>'], state)
        
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
