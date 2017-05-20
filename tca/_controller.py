# coding=utf-8
from functools import wraps
import serial
import time

from common import print_
from tca.logging_conf import get_logger


logger = get_logger(__file__)


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass


class StateException(Exception):
    """ Cannot complete the request at this time """
    pass


class BaseController(object):
    instance = None
    _status = None  # dict
    _serial = None  # serial.Serial()
    _open_retry = 60  # lines to discard when first opening the serial

    @classmethod
    def get_instance(cls, controller_config):
        """
        Get controller singleton.
        Will configure it when first called.
        
        :param controller_config: dict
        :return: BaseController singleton
        """
        if cls.instance is None:
            cls.instance = cls(**controller_config)
        return cls.instance

    def __init__(self, port, baud_rate, timeout, retries, interval, reset_disabled):
        """ Open and confirm communication, request current status """
        # TODO inherit from namedtulpe?
        self._port = port
        self._baud_rate = baud_rate
        self._timeout = timeout
        self._retries = retries
        self._reset_disabled = reset_disabled
        self._interval = interval
        self._open_serial()

    def _open_serial(self):
        """ 
        Open serial port to Arduino controller.
        Waits for acknowledgement (string "ready").
        
        :raises CommunicationException.
        """
        self._serial = serial.Serial(
            port=self._port,
            baudrate=self._baud_rate,
            timeout=self._timeout,
            dsrdtr=not self._reset_disabled,
            rtscts=not self._reset_disabled,
        )
        # self._serial.open()
        if not self._reset_disabled:
            for i in range(self._open_retry):
                line = self._serial.readline().strip()
                logger.debug("line: {}".format(line))
                if line and line == 'ready':
                    pass
                    # return  # TODO maybe wait for next readline to timeout, so we are sure this is the last data in serial pipe?
            raise CommunicationException('Cannot establish serial communication: Arduino not reporting ready')
        else:  # self._RESET_DISABLED
            pass  # TODD send a "ready?" query?

    @property
    def status(self):
        """
        Raw status from serial.
        Formatted according to common protocol.
        :return: dict
        """
        self._refresh_status()
        return self._status

    def pretty_status(self):
        """Print status dict, pretty"""
        self._refresh_status()
        for key, val in self._status.iteritems():
            print('{}: {}'.format(key, val))

    def _refresh_status(self):
        """Fetch status via serial and store it on `self._status`"""
        # TODO move implementation from FocuserController here
        # TODO fix serial protocol for dome
        raise NotImplementedError()

    def _simple_command(self, command):
        """Simple commands require no arguments are return `ok 0`"""
        # send command:
        self._serial.write('{}\n'.format(command))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()
        # TODO implement support for "ok <N>" and return the
        # response to the function
        data = []
        if not response.startswith('ok 0'):
            raise CommunicationException(
                'Controller doesnt acknowledge "{}" command, instead '
                'got: "{}"'
                    .format(command, response)
            )
        return data



def simple_cli_handler(method_name, *method_args, **method_kwargs):
    def decorator(func):
        """Actual decorator"""
        @wraps(func)
        def f(self, *args, **kwargs):
            """Wrapper body, passes `response_data` into decorated func"""
            # TODO wanna play with yield in the decorated func? "byref"
            assert isinstance(self, BaseController)
            retry = self._retries
            returned = None
            while retry:
                retry -= 1
                try:
                    method = getattr(self, method_name)
                    returned = method(*method_args, **method_kwargs)
                except:
                    time.sleep(1)  # seconds
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException(
                    'Failed "{}" after {} retries'
                    .format(method_name, self._retries)
                )
            return func(self, method_return_value=returned, *args, **kwargs)
        return f
    return decorator
