#!/usr/bin/env python
# coding=utf-8

"""Dagor focuser API {VERSION}

Usage:
    focus.py run [-m]
    focus.py -h | --help
    focus.py --version

Commands:
    run          Start the API server.

Options:
    -m --mock    Mock any hardware devices, useful for testing and development.
    -h --help    Show this screen or description of specific command.
    --version    Show version.
"""

from functools import wraps
from mock.mock import MagicMock
from docopt import docopt
from flask import Blueprint, request, make_response
from flask.ext.api.decorators import set_renderers, set_parsers
from flask_api import FlaskAPI, status as http_status
from flask_api.exceptions import ParseError

from tca import focus as dagor_focus
from tca.api import version
from tca.api.utils import (
    RegexConverter, render_error, IntBrowsableAPIRenderer,
    IntRenderer, IntParser,
    set_mock_var, read_mock_var)
from tca.logging_conf import get_logger

# noinspection PyUnboundLocalVariable
__doc__ = __doc__.format(VERSION=version)


DEFAULT_PREFIX = '/focus'

api = Blueprint('focus', __name__)

logger = get_logger('api.focus')


# Mock dagor_focus early:

def mock():
    logger.warning("*** MOCK MODE ***")
    dagor_focus_mocked = MagicMock(**{
        'get_status.return_value': {
            "can_go_up": True,
            "stopping_hard": False,
            "can_go_dn": True,
            "idle": True,
            "intent_up": False,
            "moving_up": False,
            "position": 400,
            "moving_dn": False,
            "intent_dn": False
        },
        'get_position.return_value': 400,
        'set_position.return_value': 123,
    })
    return dagor_focus_mocked

if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            set_mock_var()

if read_mock_var():
    dagor_focus = mock()


def device_repr():
    status = dagor_focus.get_status()
    return status


def position_repr():
    return dagor_focus.get_position()


# decorators:


def check_connectivity(func):
    @wraps(func)
    def func_wrapper(*args_, **kwargs_):
        try:
            return func(*args_, **kwargs_)
        except Exception as e:
            return {
                'ready': False,
                'message': '{}'.format(e),
            }, http_status.HTTP_503_SERVICE_UNAVAILABLE
    return func_wrapper


def handle_request_errors(func):
    @wraps(func)
    def func_wrapper(*args_, **kwargs_):
        try:
            return func(*args_, **kwargs_)
        except ParseError as e:
            return render_error(e)
    return func_wrapper


@api.route('/', methods=['GET', ])
@check_connectivity
def resource():
    """

    Focuser device

    ### Hyperlinks

    * [Up](..)
    * [Device state](./state/)
    * [Position](./position/)
    """
    try:
        status = dagor_focus.get_status()
    except:
        raise
    else:
        return {
            'ready': status['idle'],
        }, http_status.HTTP_200_OK


@api.route('/state/', methods=['GET', 'PUT', 'POST', ])
@check_connectivity
def state_resource():
    """
    Focuser device status.

    ###Fields

    Writable: `position`<br />
    Read-only: all other fields

    ### Methods

    **PUT**: move focuser position.<br />
    **POST**: set focuser position without moving.

    ### Hyperlinks

    * [Up](..)
    """
    if request.method in {'PUT', 'POST', }:
        posted = request.method == 'POST'
        n = request.data['position']
        if posted:
            dagor_focus.set_position(n)
        else:
            dagor_focus.goto(n)
        return (
            device_repr(),
            http_status.HTTP_200_OK if posted else http_status.HTTP_202_ACCEPTED
        )
    return device_repr()


@api.route('/position/', methods=['GET', 'PUT', 'POST', ])
@check_connectivity
@set_renderers(IntBrowsableAPIRenderer, IntRenderer)
@set_parsers(IntParser)
@handle_request_errors
def position_resource():
    """
    Simple resource that only handles position.

    ### Methods
    PUT: move focuser position.<br />
    POST: set focuser position without moving.

    ### Hyperlinks

     * [Up](..)
    """
    putted = request.method == 'PUT'
    if request.method in {'PUT', 'POST', }:
        # is body is empty, parsers never get called, so:
        if not request.content_length:
            raise ParseError('Empty request, integer required')
        # get data, via parser:
        n = request.data
        if putted:
            dagor_focus.goto(n)
        else:
            dagor_focus.set_position(n)
    n = dagor_focus.get_position()
    response = make_response(
        str(n),
        http_status.HTTP_202_ACCEPTED if putted else http_status.HTTP_200_OK)
    return response


def _run():
    app = FlaskAPI(__name__)
    app.url_map.converters['regex'] = RegexConverter
    app.debug = True
    app.register_blueprint(api, url_prefix=DEFAULT_PREFIX)
    app.run("0.0.0.0")


def _run_mocked():
    app = FlaskAPI(__name__)
    app.url_map.converters['regex'] = RegexConverter
    app.debug = True
    app.register_blueprint(api, url_prefix=DEFAULT_PREFIX)

    @app.after_request
    def print_mock(response):
        from pprint import pformat
        # noinspection PyUnresolvedReferences
        logger.debug(pformat(dagor_focus.mock_calls, indent=4))
        return response

    app.run("0.0.0.0")


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            _run_mocked()
        else:
            _run()
