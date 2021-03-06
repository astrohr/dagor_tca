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

from mock.mock import MagicMock
from docopt import docopt
from flask import Blueprint, request, make_response

from flask.ext.api.decorators import set_renderers, set_parsers
from flask_api import FlaskAPI, status as http_status
from flask_api.exceptions import ParseError

from tca import focus as dagor_focus
from tca.api import version
from tca.api.utils import (
    RegexConverter, IntBrowsableAPIRenderer,
    BoolRenderer, BoolParser,
    IntRenderer, IntParser,
    set_mock_var, read_mock_var,
    check_connectivity, retry_serial, handle_request_errors)

# noinspection PyUnboundLocalVariable
from tca.logging_conf import get_logger

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


@api.route('/', methods=['GET', ])
@check_connectivity
@retry_serial
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
@retry_serial
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
    * [Idle](../idle/)
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
@retry_serial
def position_resource():
    """
    Simple resource that only handles position.

    ### Methods
    GET: show current focus.<br />
    PUT: move focuser position.<br />
    POST: set focuser position without moving.

    ### Hyperlinks

     * [Up](..)
     * [Idle](../idle/)
    """
    putted = request.method == 'PUT'
    if request.method in {'PUT', 'POST', }:
        # is body is empty, parsers never get called, so:
        if not request.content_length:
            raise ParseError('Empty request, integer required')
        # get data, via parser:
        n = request.data
        if putted:
            dagor_focus.get_controller()._step_to(n)
        else:
            dagor_focus.set_position(n)
    n = dagor_focus.get_position()
    response = make_response(
        str(n),
        http_status.HTTP_202_ACCEPTED if putted else http_status.HTTP_200_OK)
    return response


@api.route('/idle/', methods=['GET', ])
@check_connectivity
@set_renderers(IntBrowsableAPIRenderer, BoolRenderer)
@set_parsers(BoolParser)
@handle_request_errors
@retry_serial
def idle_resource():
    """
    Simple resource that only handles "idle" status.

    ### Methods
    GET: show current focus.

    ### Hyperlinks

     * [Up](..)
     * [Position](../position/)
    """
    idle = dagor_focus.get_idle()
    response = make_response(
        str(idle),
        http_status.HTTP_200_OK)
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
