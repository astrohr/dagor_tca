#!/usr/bin/env python
# -*- coding: UTF-8 -*-


import serial


class DeviceException(Exception):
    pass


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
    def status(self):
        self._refresh_status()
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
        self._refresh_status()

    def pretty_status(self):
        self._refresh_status()
        print('number of switches: {}'.format(self._n))
        for i in range(self._n):
            print(
                "switch {}: {}".format(i + 1,
                                       "ON" if self._status[i] else "OFF"))

    def switch(self, n, state):
        if n == 'all':
            for i in range(0, self._n):
                self._switch(i + 1, state)
        else:
            self._switch(int(n), state)

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

        while self._serial.read():
            pass
        if not self.RESET_DISABLED:
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
                'Controller doesnt acknowledge switch command, instead got: {}'.format(
                    response))
        response = self._serial.readline().strip()
        if response not in ('ON', 'OFF'):
            raise CommunicationException(
                'Expected "ON" or "OFF", got: {}'.format(response))
        self._status[n - 1] = True if response == 'ON' else False


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass
