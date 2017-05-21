# coding=utf-8
"""Base Controller and some exceptions related to hardware."""
import serial
import time

from common import EnterAbort, print_, wait_for_time
from tca.logging_conf import get_logger

logger = get_logger(__file__)


class CommunicationException(Exception):
    """ There was an error communicating with Arduino controller """
    pass


class StateException(Exception):
    """ Cannot complete the request at this time """
    pass


class BaseController(object):
    """
    Base for hardware controllers using "common" serial protocol.
    """
    instance = None
    _status = None  # dict
    _serial = None  # serial.Serial()
    _open_retry = 60  # lines to discard when first opening the serial
    # TODO put firmware version in status:
    _default_status = {}
    _default_status_type = {}

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
        data = self._simple_command('\n\n\n\n\n\n\n\n\nstatus')
        # verify status response:
        new_status = {}
        for line in data:
            key, value = line.split(' ')
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
                    .format(new_status.keys(),
                            self._default_status.keys()))
        # keep new status:
        self._status = new_status
        # ... and return it, sometimes useful:
        return new_status

    def _simple_command(self, command):
        """Simple commands require no arguments are return `ok 0`"""
        # send command:
        self._serial.write('{}\n'.format(command))
        # read first response line:
        response = self._serial.readline().strip()  # expect: "ok 0\n"
        if not response:  # blank line
            response = self._serial.readline().strip()

        # TODO actually read error info from serial

        # response, first line:
        valid_confirmations = ('ok', command.strip(), )
        confirmation = ''
        for c in valid_confirmations:
            if response.startswith(c + ' '):
                confirmation = c
        if not confirmation:
            raise CommunicationException(
                'Controller doesnt acknowledge "{}" command, instead got: "{}"'
                .format(command, response))

        # response, next N lines:
        data = []
        try:
            n = int(response[len(confirmation) + 1:])
        except ValueError:
            raise CommunicationException(
                'Expected int, got: "{}"'.format(response))
        for _ in range(n):
            line = self._serial.readline().strip()
            data.append(line)

        return data


class CliMixin(object):
    """
    Mixin for Controllers that handles CLI interactions.
    """
    _status = {}
    _retries = 0
    _interval = 1  # seconds

    def __init__(self, retries, interval, **kwargs):
        self._retries = retries
        self._interval = interval
        # noinspection PyArgumentList
        super(CliMixin, self).__init__(**kwargs)

    def pretty_status(self):
        """Print status, prettily"""
        if hasattr(self, '_refresh_status'):
            self._refresh_status()
        self._print_pretty_status()

    def _print_pretty_status(self):
        """Print status dict, pretty"""
        for key, val in self._status.iteritems():
            print('{}: {}'.format(
                self.prettify_key(key, val),
                self.prettify_val(key, val),
            ))

    # noinspection PyUnusedLocal,PyMethodMayBeStatic
    def prettify_key(self, key, val):
        """Prettify a key, optionally."""
        return key

    # noinspection PyUnusedLocal,PyMethodMayBeStatic
    def prettify_val(self, key, val):
        """Prettify a value, optionally."""
        return val

    def _simple_cli_handler(self, func):
        """Ruins a callable until no exception, up to `self._retries` times"""
        retry = self._retries
        while retry:
            retry -= 1
            try:
                returned = func()
            except Exception:
                time.sleep(self._interval)
            else:
                return returned
        if retry == 0:
            raise CommunicationException(
                'Failed calling "{}" after {} retries'
                .format(func, self._retries)
            )

    def _dots_cli_handler(
            self,
            func_start,
            func_on_target=None,
            func_reached_target=None,
            func_fail=None,
            func_stop=None,
            func_finalize=None):
        """
        CLI handler that provides "dots" feature.
        For example: positioning..............
        
        :param func_start: callable, starts the process, required.  
        :param func_on_target: callable, check is target reached, optional.
        :param func_reached_target: callable, executes when target reached
        :param func_fail: callable, executes if an error occurs, optional. 
        :param func_stop: callable, executes on manual stop, optional.
                          If set to `False`, enter key wont abort execution.
        :param func_finalize: callable, executes last, always, optional.
        """
        if func_on_target is None:
            func_on_target = lambda: False  # False == drive forever
        if func_reached_target is None:
            func_reached_target = lambda: None
        if func_fail is None:
            func_fail = lambda: None
        if func_stop is None:
            func_stop = lambda: print_('')
        if func_finalize is None:
            func_finalize = lambda: None
        try:
            retry = self._retries
            while retry:
                retry -= 1
                try:
                    func_start()
                except Exception as e:
                    logger.info("stopping, caught {}".format(e))
                    func_fail()
                    time.sleep(self._interval)
                else:
                    break  # from while
            if retry == 0:
                raise CommunicationException(
                    'Failed calling "{}" after {} retries'
                    .format(func_start, self._retries)
                )
            on_target = False
            enter_abort = func_stop is not False
            while not on_target:
                wait_for_time(self._interval, dots=True,
                              enter_abort=enter_abort,
                              end='')
                on_target = func_on_target()
        except (KeyboardInterrupt, EnterAbort):
            func_stop()
        else:
            func_reached_target()
        finally:
            func_finalize()
