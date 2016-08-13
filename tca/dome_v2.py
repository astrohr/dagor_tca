#!/usr/bin/env python
"""Dagor dome rotation interface version 2.0.

Usage:
    dome.py server
    dome.py status [-v | --verbose]
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
    status         Dome status (azimuth \\n rotating or not \\n calibrated, during calibration or uncalibrated).
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


from __future__ import print_function, division, absolute_import
from tca.common import EnterAbort, wait_for_time, print_, exit_

from docopt import docopt
import serial
import sys
import time

from tca._utils import str_bool
from tca.logging_conf import get_logger
from tca.local import dome_config

logger = get_logger(__file__)

RESET_DISABLED = True

SERIAL = {
    'PORT': dome_config.PORT,
    'BAUDRATE': 9600,
    'TIMEOUT': 2,  # if no data available, block for max this many seconds
}

RETRIES = 5
INTERVAL = 1  # seconds


class DomeController(object):
    """ Controls the Dome, responds to serial/USB communication.

        Note: it always requests latest status from Arduino controller.
        
     .. Serial settings: 
        9600 baud, newline line ending

     .. Protocol:
         TODO describe protocol
    """

    instance = None

    @classmethod
    def get_instance(cls):
        if cls.instance is None:
            cls.instance = cls()
        return cls.instance

    # public API

    def __init__(self, SERIAL=SERIAL, RESET_DISABLED=RESET_DISABLED):
        """ Open and confirm communication, request current status """
        self._SERIAL = SERIAL
        self._RESET_DISABLED = RESET_DISABLED
        self._open_serial()

    @property
    def status(self):
        self._refresh_status()
        return self._status

    def pretty_status(self):
        self._refresh_status()
        for key, val in self._status.iteritems():
            print('{}: {}'.format(key, val))

    def calibrate(self):

    def go_to(self, azimuth):

    def set_azim(self, azimuth):

    def get_azim(self):
    # Maybe better as a property "azimuth"

    def park(self):

    def door_open(self):

    def door_close(self):

    def door_stop(self):

    def set_home(self, azimuth):

    def get_home(self):
    # Maybe better as a property "home"

    def slew_left(self):

    def slew_right(self):

    def slew_abort(self):


    # private members


    return_str = 'Azimuth: ' + str(position) + "\n" + 'Rotation: ' + {'0': 'Stop', '1': 'Down', '-1': 'Up'}[
        rotation] + "\n" + 'Calibration: ' + {'0': 'In progress', '1': 'Done', '-1': 'Not calibrated'}[calibration]


    # Rotation (Slew):
    #   0: Stop
    #   1: Down (Right)
    #  -1: Up (Left)

    # Calibration:
    #   0: In progress
    #   1: Done
    #  -1 Not calibrated

    _status = None
    _default_status = {
        'azimuth': 0,
        'slew': 0,
        'calibration': 0,
    }
    _default_status_type = {
        'azimuth': int,
        'slew': str_bool,
        'calibration': str_bool,
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

    def _calibrate(self):

    def _go_to(self, azimuth):

    def _set_azim(self, azimuth):

    def _get_azim(self):

    def _park(self):

    def _door_open(self):

    def _door_close(self):

    def _door_stop(self):

    def _set_home(self, azimuth):

    def _get_home(self):

    def _slew_left(self):

    def _slew_right(self):

    def _slew_abort(self):




def get_controller():
    return DomeController.get_instance()


def get_status():
    return get_controller().status


def get_position():
    return get_controller().get()


def set_position(n):
    n = int('{}'.format(n))
    return get_controller().set(n)


def goto(n):
    n = int('{}'.format(n))
    return get_controller().step_to(n)


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass


class StateException(Exception):
    """ Cannot complete the request at this time """
    pass


def _main(args):
    controller = get_controller()

    #TODO Parse agruments

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
