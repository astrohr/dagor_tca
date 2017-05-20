# coding=utf-8
import serial
import time

from common import EnterAbort, print_, _wait_for_time
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

    def __init__(self, port, baud_rate, timeout, reset_disabled):
        """ Open and confirm communication, request current status """
        # TODO inherit from namedtulpe?
        self._port = port
        self._baud_rate = baud_rate
        self._timeout = timeout
        self._reset_disabled = reset_disabled
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
                    # TODO maybe wait for next readline to timeout?
                    # ...  so we are sure this is the last data in serial pipe
            raise CommunicationException(
                'Cannot establish serial communication:'
                ' Arduino not reporting ready')
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
                'Controller doesnt acknowledge "{}" command, instead got: "{}"'
                .format(command, response)
            )
        return data


class CliMixin(object):
    _status = {}
    _retries = 0
    _interval = 1  # seconds

    def __init__(self, retries, interval, **kwargs):
        self._retries = retries
        self._interval = interval
        # noinspection PyArgumentList
        super(CliMixin, self).__init__(**kwargs)

    def pretty_status(self):
        """Print status dict, pretty"""
        for key, val in self._status.iteritems():
            print('{}: {}'.format(key, val))

    def _simple_cli_handler(self, func):
        """Ruins a callable untill no exception, up to `self._retries` times"""
        retry = self._retries
        while retry:
            retry -= 1
            try:
                returned = func()
            except:
                time.sleep(self._interval)
            else:
                return returned
        if retry == 0:
            raise CommunicationException(
                'Failed calling "{}" after {} retries'
                .format(func, self._retries)
            )

    def _dots_cli_handler(self, func_start, func_stop=None):
        if func_stop is None:
            func_stop = lambda: None
        try:
            retry = self._retries
            while retry:
                retry -= 1
                try:
                    func_start()
                except Exception as e:
                    logger.info("stopping, caught {}".format(e))
                    func_stop()
                    time.sleep(self._interval)
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException(
                    'Failed calling "{}" after {} retries'
                    .format(func_start, self._retries)
                )
            while True:
                _wait_for_time(self._interval, dots=True,
                               enter_abort=True,
                               end='')
        except (KeyboardInterrupt, EnterAbort):
            print('stoppingG')
            func_stop()
