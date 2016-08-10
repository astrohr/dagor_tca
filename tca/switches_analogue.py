#!/usr/bin/env python
# -*- coding: UTF-8 -*-


import serial


class DeviceException(Exception):
    pass


class AnalogueSwitchController(object):
    """ Controls one or more switches, responds to serial/USB communication.

        Note: it always requests latest status from Arduino controller.
        
     .. Serial settings: 
        9600 baud, newline line ending

     .. Protocol:
         .. Query:
            Single line
            Options: 
             * status
             * switch <N> (0|1|2)
             * switch all (0|1|2)
             * reset
    """

    # TODO: This should be base class for all other controllers

    PORT = None
    BAUDRATE = 9600
    TIMEOUT = 0.1  # seconds
    RESET_DISABLED = True

    # public API
    @property
    def n(self):
        return self._n

    @property
    def status_analogue(self):
        self._refresh_status_analogue()
        return self._status

    def __init__(self):
        """ Open and confirm communication, request current status """
        if self.PORT is None:
            raise ValueError(
                "PORT not provided. Subclass SwitchController and "
                "specify PORT and RESET_DISABLED")
        if self.RESET_DISABLED is None:
            raise ValueError(
                "RESET_DISABLED not provided. Subclass SwitchController and "
                "specify PORT and RESET_DISABLED")

        self._SERIAL = {
            'PORT': self.PORT,
            'BAUDRATE': self.BAUDRATE,
            # if no data available, block for max this many seconds:
            'TIMEOUT': self.TIMEOUT,
        }
        self._open_serial()
        self._refresh_status_analogue()

    # TODO: Make generic
    def pretty_status_analogue(self):
        self._refresh_status_analogue()
        print('number of switches: {}'.format(self._n))
        for i in range(self._n):
            if self._status[i] == 0:
                print("switch {}: {}".format(i + 1, "OFF"))
            elif self._status[i] == 1:
                print("switch {}: {}".format(i + 1, "Working state 1"))
            else:
                print("switch {}: {}".format(i + 1, "Working state 2"))


    # TODO: Make generic
    def switch_analogue(self, n, state):
        if n == 'all':
            for i in range(0, self._n):
                self._switch_analogue(i + 1, state)
        else:
            self._switch_analogue(int(n), state)

    # private members

    _n = 0  # number of swiotches available to this controller
    _status = []  # state of each switch: True for ON, False for OFF
    _serial = None  # serial.Serial()

    def _open_serial(self):
        """ Open serial port to Arduino controller. Waits for acknowlegement (string "ready").
            Throws CommunicationException.
        """
        self._serial = serial.Serial()
        self._serial.baudrate = self._SERIAL['BAUDRATE']
        self._serial.timeout = self._SERIAL['TIMEOUT']

        def open_first_available_port():
            """USB port (/dev/tty*) keeps changing, try multiple devices"""
            devices = self._SERIAL['PORT'][:]
            # but don't break if one port is specified (string, not a list):
            if isinstance(devices, basestring):
                devices = [devices, ]
            # open first device that works:
            while devices:
                device = devices.pop()
                self._serial.port = device
                try:
                    self._serial.open()
                except serial.SerialException:
                    # re-raise if this is last port:
                    if not devices:
                        raise
                else:
                    # successfully opened:
                    return
        try:
            open_first_available_port()
        except serial.SerialException as e:
            raise CommunicationException(
                'Cannot connect to Arduino: {}'.format(e))

        a_char = ""
        for _ in range(40):
            a_char = self._serial.read()
            if a_char.strip():
                break
        if not self.RESET_DISABLED:
            line = a_char + self._serial.readline().strip()
            if line and line == 'ready':
                return  # TODO maybe wait for next readline to timeout, so we are sure this is the last data in serial pipe?
            raise CommunicationException(
                'Cannot establish serial communication: Arduino not reporting ready')
        else:  # self._RESET_DISABLED
            pass  # TOOD send a "ready?" query?


    # TODO: Make generic
    def _refresh_status_analogue(self):
        self._serial.write('status\n')
        # drop any leading blank lines:
        response = ""
        for _ in range(5):
            response = self._serial.readline().strip()
            if response.strip():
                break
        if response != 'status 2':
            raise CommunicationException(
                'Controller doesnt acknowlege status command, instead got: {}'.format(
                    response))

        self._status = []

        # Read status for central fan --> expected "Fan1: <status_code>"

        response = self._serial.readline().strip()
        if not response.startswith("fan 1: "):
            raise CommunicationException(
                'Expected "fan 1: <status_code>", got: {}'.format(response))

        fan_status = response.split(":")
        status_code = fan_status[1].strip()
        if status_code not in ('0', '1', '2'):
            raise CommunicationException(
                'Expected "0", "1" or "2", got: {}'.format(response))

        self._status.append(status_code)

        # Read status for other fans --> expected "Fan2: <status_code>"

        response = self._serial.readline().strip()
        if not response.startswith("fan 2: "):
            raise CommunicationException(
                'Expected "fan 2: <status_code>", got: {}'.format(response))

        fan_status = response.split(":")
        status_code = fan_status[1].strip();
        if status_code not in ('0', '1', '2'):
            raise CommunicationException(
                'Expected "0", "1" or "2", got: {}'.format(response))

        self._status.append(status_code)

    def _switch_analogue(self, n, state):
        self._serial.write('fan {} set {}\n'.format(n, state))

        response = self._serial.readline().strip()
        # drop any leading blank lines:
        while not response.strip():
            response = self._serial.readline().strip()

        if response != 'ok 1':
            raise CommunicationException(
                'Controller doesnt acknowledge fan command, instead got: {}'.format(
                    response))

        response = self._serial.readline().strip()
        if response not in ('0', '1', '2'):
            raise CommunicationException(
                'Expected "0", "1" or "2", got: {}'.format(response))
        self._status[n - 1] = response

        #return 'Fan {} set to state {}'.format(n, response)



class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass
