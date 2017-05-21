#!/usr/bin/env python
# coding=utf-8
"""Dagor focus interface version 0.9.0

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

import sys
from functools import partial

from docopt import docopt

from _controller import (
    CliMixin,
    BaseController,
    CommunicationException,
    StateException,
)
from _utils import str_bool
from common import print_, exit_
from local.configuration import FOCUS_CONFIG
from logging_conf import get_logger

logger = get_logger(__file__)


class FocuserController(CliMixin, BaseController):
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
    def idle(self):
        self._refresh_status()
        return self._status['idle']

    @property
    def can_go_up(self):
        self._refresh_status()
        return self._status['can_go_up']

    def step_to(self, n):
        print_('positioning', end='')
        self._dots_cli_handler(
            func_start=partial(self._step_to, n),
            func_stop=self.stop,
            func_on_target=lambda: self._refresh_status()['idle'],
            func_reached_target=lambda: print_(
                '\nposition: {}'.format(self._status['position'])),
        )

    def step_by(self, n):
        print_('positioning', end='')
        self._dots_cli_handler(
            func_start=partial(self._step_by, n),
            func_stop=self.stop,
            func_on_target=lambda: self._refresh_status()['idle'],
            func_reached_target=lambda: print_(
                '\nposition: {}'.format(self._status['position'])),
        )

    def stop(self):
        print_('stopping', end='')
        self._dots_cli_handler(
            func_start=self._stop,
            func_stop=False,
            func_on_target=lambda: self._refresh_status()['idle'],
            func_reached_target=lambda: print_(
                '\nposition: {}'.format(self._status['position'])),
        )

    def set(self, n):
        self._set(n)

    def get(self):
        return self._get()

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

    def _step_to(self, n):
        self._refresh_status()
        if not self._status['idle']:
            logger.debug("not idle")
            raise StateException('Motor not idle')
        self._simple_command('\n\n\n\n\n\n\n\n\n\nstep to {}'.format(n))

    def _step_by(self, n):
        self._refresh_status()
        if not self._status['idle']:
            logger.debug("not idle")
            raise StateException('Motor not idle')
        # send command:
        self._simple_command('\n\n\n\n\n\n\n\n\n\nstep by {}'.format(n))

    def _stop(self):
        logger.debug("stop")
        self._simple_command('\n\n\n\n\n\n\n\n\n\nstop')

    def _set(self, n):
        n = int(n)
        # TODO move these \n to simple command (when fix dome)
        data = self._simple_command(
            '\n\n\n\n\n\n\n\n\n\nposition set {}'.format(n))
        position = int(data[0])
        if not position == n:
            raise CommunicationException(
                'Controller acknowledges "set" command, '
                'but returned wrong position: {}'.format(position))

    def _get(self):
        # send command:
        data = self._simple_command('\n\n\n\n\n\n\n\n\n\nposition')
        try:
            n = int(data[0])
        except (ValueError, IndexError):
            raise CommunicationException(
                'Controller acknowledges "get" command, '
                'but returned something strange: {}'.format(data))
        return n


def get_controller():
    return FocuserController.get_instance(FOCUS_CONFIG)


def get_status():
    return get_controller().status


def get_idle():
    return get_controller().idle


def get_position():
    return get_controller().get()


def set_position(n):
    n = int('{}'.format(n))
    get_controller().set(n)


def goto(n):
    """
    Move the focuser to specified position.
    :param n: integer or string 
    """
    n = int('{}'.format(n))
    get_controller().step_to(n)


def _main(args):
    controller = get_controller()
    assert isinstance(controller, FocuserController)

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
        print(controller.get())

    exit(0)


if __name__ == '__main__':
    cli_args = docopt(
        __doc__, version=__doc__.split('\n'[0]), options_first=True)
    if len(sys.argv) == 1:
        print(__doc__.strip())
        exit(0)

    try:
        _main(cli_args)
    except Exception as e:
        logger.warning(e, exc_info=True)
        # raise
        exit_('ERROR')
