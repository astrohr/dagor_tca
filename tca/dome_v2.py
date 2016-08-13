#!/usr/bin/env python
"""Dagor dome rotation interface version 2.0.

Usage:
    dome.py status
    dome.py calibrate
    dome.py go_to <azimuth>
    dome.py azimuth set <azimuth>
    dome.py azimuth get
    dome.py park
    dome.py door (open|close|stop)
    dome.py home (set|get)
    dome.py slew (left|right|abort)
    dome.py -h | --help
    dome.py --version

Commands:
    status         Dome status.
    calibrate      Start calibration.
    go_to          Move dome to specified azimuth.
    azimuth set    Set specified azimuth as current position.
    azimuth get    Get current azimuth.
    park           Move dome to home azimuth.
    door           Open, close or stop the door.
    home set       Set home to current azimuth.
    home get       Get home azimuth.
    slew left      Start rotation of the dome (south-east-north-west).
    slew right     Start rotation of the dome (south-wast-north-east).
    slew abort     Stop rotation of the dome.

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
    @property
    def status(self):
        self._refresh_status()
        return self._status

    # TODO: Door/Dome moving check implemented in calibrate, go_to, set_azimuth and park functions as an example
    # TODO: Discuss if needed for other operations as well

    def __init__(self, SERIAL=SERIAL, RESET_DISABLED=RESET_DISABLED):
        """ Open and confirm communication, request current status """
        self._SERIAL = SERIAL
        self._RESET_DISABLED = RESET_DISABLED
        self._open_serial()

    def pretty_status(self):
        self._refresh_status()
        for key, val in self._status.iteritems():
            print('{}: {}'.format(key, val))

    def calibrate(self):
        try:
            retry = RETRIES
            while retry:
                retry -= 1
                try:
                    self._calibrate()
                except DoorException:
                    logger.info("Door not idle, stopping")
                    self._door_stop()
                    time.sleep(1)  # seconds
                except SlewException:
                    logger.info("Dome not idle, aborting")
                    self._slew_abort()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException('Failed "go_to" after {} retries'
                                             .format(RETRIES))
            print_("Calibrating.", end='')
            self._refresh_status()
            # Check status until the dome and door stop slewing/moving
            while (not self._status['slew'] == 0) or (self._status['door'] == 4):
                _wait_for_time(INTERVAL, dots=True,
                               enter_abort=True,
                               end='')
                self._refresh_status()
            print()
            print('Successfully calibrated.')
        except (KeyboardInterrupt, EnterAbort):
            ##TODO What to do here?
            print()

    def go_to(self, azimuth):
        try:
            retry = RETRIES
            while retry:
                retry -= 1
                try:
                    self._go_to(azimuth)
                except DoorException:
                    logger.info("Door not idle, stopping")
                    self._door_stop()
                    time.sleep(1)  # seconds
                except SlewException:
                    logger.info("Dome not idle, aborting")
                    self.slew_abort()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException('Failed "go_to" after {} retries'
                                             .format(RETRIES))
            print_("Slewing.", end='')
            self._refresh_status()
            # Check status until the dome and door stop slewing/moving
            while (not self._status['slew'] == 0) or (self._status['door'] == 4):
                _wait_for_time(INTERVAL, dots=True,
                               enter_abort=True,
                               end='')
                self._refresh_status()
            print()
            print('position (azimuth): {}'.format(self._status['azimuth']))
        except (KeyboardInterrupt, EnterAbort):
            self.slew_abort()

    def set_azimuth(self, azimuth):
        #TODO Discuss - same es goto?
        try:
            retry = RETRIES
            while retry:
                retry -= 1
                try:
                    self._set_azimuth(azimuth)
                except DoorException:
                    logger.info("Door not idle, stopping")
                    self._door_stop()
                    time.sleep(1)  # seconds
                except SlewException:
                    logger.info("Dome not idle, aborting")
                    self.slew_abort()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException('Failed "go_to" after {} retries'
                                             .format(RETRIES))
            print_("Setting azimuth.", end='')
            self._refresh_status()
            # Check status until the dome and door stop slewing/moving
            while (not self._status['slew'] == 0) or (self._status['door'] == 4):
                _wait_for_time(INTERVAL, dots=True,
                               enter_abort=True,
                               end='')
                self._refresh_status()
            print()
            print('Position (azimuth): {}'.format(self._status['azimuth']))
        except (KeyboardInterrupt, EnterAbort):
            self.slew_abort()

    def get_azimuth(self):
        return self._get_azimuth()

    def park(self):
        try:
            retry = RETRIES
            while retry:
                retry -= 1
                try:
                    self._park()
                except DoorException:
                    logger.info("Door not idle, stopping")
                    self._door_stop()
                    time.sleep(1)  # seconds
                except SlewException:
                    logger.info("Dome not idle, aborting")
                    self.slew_abort()
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException('Failed "go_to" after {} retries'
                                             .format(RETRIES))
            print_("Parking.", end='')
            self._refresh_status()
            # Check status until the dome and door stop slewing/moving
            while (not self._status['slew'] == 0) or (self._status['door'] == 4):
                _wait_for_time(INTERVAL, dots=True,
                               enter_abort=True,
                               end='')
                self._refresh_status()
            print()
            print('Parked successfully')
        except (KeyboardInterrupt, EnterAbort):
            self.slew_abort()

    def door_open(self):
        self._door_open()

    def door_close(self):
        self._door_close()

    def door_stop(self):
        self._door_stop()

    def set_home(self):
        self._set_home()

    def get_home(self):
        return self._get_home()

    def slew_left(self):
        self._slew_left()

    def slew_right(self):
        self._slew_right()

    def slew_abort(self):
        self._slew_abort()

    # private members

    ## Status Codes:
    #
    ## Rotation (Slew):
    #   0: Stop
    #   1: Down (Right)
    #  -1: Up (Left)
    #
    ## Door
    #   0: Closed
    #   1: Opened
    #   3: Unknown state # Stopped at some point during open/close
    #   4: Moving
    #
    ## Calibration:
    #   0: In progress
    #   1: Done
    #  -1 Not calibrated

    _status = None
    _default_status = {
        'azimuth': 0,
        'slew': 0,
        'door': 0,
        'calibration': -1,
    }
    _default_status_type = {
        'azimuth': int,
        'slew': int,
        'door': int,
        'calibration': int,
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

    def _calibrate(self):
        self._refresh_status()

        # Check if the door is moving
        if self._status['door'] == 4:
            logger.debug("not idle - door is moving")
            raise DoorException('Door not idle')

        # Check if the dome is slewing
        if not self._status['slew'] == 0:
            logger.debug("not idle - dome is slewing")
            raise StateException('Dome not idle')

        # send command:
        self._serial.write('calibrate\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "calibrate" command, instead got: {}'.format(response))


    def _go_to(self, azimuth):
        self._refresh_status()

        # Check if the door is moving
        if self._status['door'] == 4:
            logger.debug("not idle - door is moving")
            raise DoorException('Door not idle')

        # Check if the dome is slewing
        if not self._status['slew'] == 0:
            logger.debug("not idle - dome is slewing")
            raise StateException('Dome not idle')

        # send command:
        self._serial.write('goto {}\n'.format(azimuth))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "goto" command, instead got: {}'.format(response))


    def _set_azimuth(self, azimuth):
        self._refresh_status()

        # Check if the door is moving
        if self._status['door'] == 4:
            logger.debug("not idle - door is moving")
            raise DoorException('Door not idle')

        # Check if the dome is slewing
        if not self._status['slew'] == 0:
            logger.debug("not idle - dome is slewing")
            raise StateException('Dome not idle')

        # send command:
        self._serial.write('force {}\n'.format(azimuth))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "goto" command, instead got: {}'.format(response))

    def _get_azimuth(self):
        self._refresh_status()

        #TODO: Dicsuss: not necessary - we can read this directly from status
        # send command:
        #self._serial.write('current_azimuth\n'.format(azimuth))

        return self._status['azimuth']

    def _park(self):
        self._refresh_status()

        # Check if the door is moving
        if self._status['door'] == 4:
            logger.debug("not idle - door is moving")
            raise DoorException('Door not idle')

        # Check if the dome is slewing
        if not self._status['slew'] == 0:
            logger.debug("not idle - dome is slewing")
            raise StateException('Dome not idle')

        # send command:
        self._serial.write('park\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "park" command, instead got: {}'.format(response))

    def _door_open(self):
        # send command:
        self._serial.write('door_open\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "door_open" command, instead got: {}'.format(response))

    def _door_close(self):
        # send command:
        self._serial.write('door_close\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "door_close" command, instead got: {}'.format(response))

    def _door_stop(self):
        # send command:
        self._serial.write('door_stop\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "door_stop" command, instead got: {}'.format(response))

    def _set_home(self):
        # send command:
        self._serial.write('set_as_home\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "set_as_home" command, instead got: {}'.format(response))

    def _get_home(self):
        # send command:
        self._serial.write('home_azimuth\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 1\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 1'):
            raise CommunicationException('Controller doesnt acknowledge "home_azimuth" command, instead got: {}'.format(response))

        response = self._serial.readline().strip()  # expect: position number
        try:
            n = int(response)
        except ValueError:
            raise CommunicationException(
                'Controller acknowledges "home_azimuth" command, but returned something strange: {}'.format(
                    response))
        return n

    def _slew_left(self):
        # send command:
        self._serial.write('up\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "up" command, instead got: {}'.format(response))

    def _slew_right(self):
        # send command:
        self._serial.write('down\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "down" command, instead got: {}'.format(response))

    def _slew_abort(self):
        # send command:
        self._serial.write('stop\n')
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        if not response.startswith('ok 0'):
            raise CommunicationException('Controller doesnt acknowledge "stop" command, instead got: {}'.format(response))


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass

class StateException(Exception):
    """ Cannot complete the request at this time """
    pass

class SlewException(Exception):
    """ Cannot complete the request at this time - dome is slewing """
    pass

class DoorException(Exception):
    """ Cannot complete the request at this time - door is moving"""
    pass

def get_controller():
    return DomeController.get_instance()

def get_status():
    return get_controller().status


def _main(args):
    controller = get_controller()

    if args['status']:
        controller.pretty_status()
    elif args['calibrate']:
        controller.calibrate()
    elif args['go_to']:
        azimuth = int(args['<azimuth>'])
        controller.go_to(azimuth)
    elif args['azimuth'] and args['set']:
        azimuth = int(args['<azimuth>'])
        controller.set_azimuth(azimuth)
    elif args['azimuth'] and args['get']:
        print(controller.get_azimuth())
    elif args['park']:
        controller.park()
    elif args['door'] and args['opem']:
        controller.door_open()
    elif args['door'] and args['close']:
        controller.door_close()
    elif args['door'] and args['stop']:
        controller.door_stop()
    elif args['home'] and args['set']:
        azimuth = int(args['<azimuth>'])
        controller.set_home(azimuth)
    elif args['home'] and args['get']:
        print(controller.get_home())
    elif args['slew'] and args['left']:
        controller.slew_left()
    elif args['slew'] and args['right']:
        controller.slew_right()
    elif args['slew'] and args['abort']:
        controller.slew_abort()

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
