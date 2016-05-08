#!/usr/bin/env python
"""Dagor switches interface version 0.8.

Usage:
    switches.py status
    switches.py switch <N> (on|off)
    switches.py switch all (on|off)
    switches.py reset
    switches.py -h | --help
    switches.py --version

Commands:
    status         Display status of switches
    switch         Switch a particular switch on or off. Sepcify "all" to affect all available switches. (NOT IMPLEMENTED9
    reset          Reset Arduino. (NOT IMPLEMENTED)

Options:
    -h --help      Show this screen or description of specific command.
    --version      Show version.
"""

from docopt import docopt
import serial
import sys
from time import sleep

RESET_DISABLED = True
SERIAL = {
    # 'PORT' : '/dev/ttyUSB0',  # TCS
    'PORT' : '/dev/ttyUSB1',  # TCS
    #'PORT': '/dev/tty.usbserial-A703F1A5',
    'BAUDRATE': 9600,
    'TIMEOUT': 0.1,  # if no data available, block for max this many seconds
}


class SwitchController(object):
    """ Controls one or more switches, responds to serial/USB communication.

        Note: it always requests latest status from Arduino controller.
        
     .. Serial settings: 
        9600 baud, newline line ending

     .. Protocol:
         .. Query:
            Single line
            Options: 
             * status
             * switch <N> (on|off)
             * switch all (on|off)
             * reset
    """

    # public API
    @property
    def n(self):
        return self._n

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
        print('number of switches: {}'.format(self._n))
        for i in range(self._n):
            print(
            "switch {}: {}".format(i + 1, "ON" if self._status[i] else "OFF"))

    def switch(self, n, state):
        if n == 'all':
            for i in range(0, self._n):
                self._switch(i + 1, state)
        else:
            self._switch(int(n), state)

    # peivate members

    _n = 0  # number of swiotches available to this controller
    _status = []  # state of each switch: True for ON, False for OFF 
    _serial = None  # serial.Serial()

    def _open_serial(self):
        """ Open serial port to Arduino controller. Waits for acknowlegement (string "ready").
            Throws CommunicationException.
        """
        self._serial = serial.Serial()
        self._serial.port = self._SERIAL['PORT']
        self._serial.baudrate = self._SERIAL['BAUDRATE']
        self._serial.timeout = self._SERIAL['TIMEOUT']
        self._serial.open()
        while self._serial.read():
            pass
        if not self._RESET_DISABLED:
            for i in range(6):
                line = self._serial.readline().strip()
                if line and line == 'ready':
                    return  # TODO maybe wait for next readline to timeout, so we are sure this is the last data in serial pipe?
            raise CommunicationException(
                'Cannot establish serial communication: Arduino not reporting ready')
        else:  # self._RESET_DISABLED
            pass  # TOOD send a "ready?" query?

    def _refresh_status(self):
        self._serial.write('status\n')
        response = self._serial.readline().strip()
        if response != 'status':
            raise CommunicationException(
                'Controller doesnt acknowlege status command, instead got: {}'.format(
                    response))
        response = self._serial.readline().strip()
        try:
            self._n = int(response)
        except ValueError:
            raise CommunicationException(
                'Expected int, got: {}'.format(response))
        self._status = []
        for _ in range(self._n):
            response = self._serial.readline().strip()
            if response in ("ON", "OFF"):
                self._status.append(response == "ON")
            else:
                raise CommunicationException(
                    'Expected "ON" or "OFF", got: {}'.format(response))

    def _switch(self, n, state):
        on_off = 'on' if state else 'off'
        self._serial.write('switch {} {}\n'.format(n, on_off))
        response = self._serial.readline().strip()
        if response != 'switch {}'.format(n):
            raise CommunicationException(
                'Controller doesnt acknowlege switch command, instead got: {}'.format(
                    response))
        response = self._serial.readline().strip()
        if response not in ('ON', 'OFF'):
            raise CommunicationException(
                'Expected "ON" or "OFF", got: {}'.format(response))
        self._status[n - 1] = True if response == 'ON' else False


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass


def _main(args):
    controller = SwitchController(SERIAL, RESET_DISABLED)

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
        # exit_('ERROR')
