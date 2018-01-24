#!/usr/bin/env python
# coding=utf-8

"""Dagor telescope API {VERSION}

Usage:
    telescope.py run [-m]
    telescope.py -h | --help
    telescope.py --version

Commands:
    run          Start the API server.

Options:
    -m --mock    Mock any hardware devices, useful for testing and development.
    -h --help    Show this screen or description of specific command.
    --version    Show version.
"""

from mock.mock import MagicMock
from docopt import docopt
from datetime import datetime, timedelta
from flask import Blueprint, request, make_response

from common import p_
from flask.ext.api.decorators import set_renderers, set_parsers
from flask_api import FlaskAPI, status as http_status
from flask_api.exceptions import ParseError

from tca import track as dagor_track
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


DEFAULT_PREFIX = '/telescope'

api = Blueprint('telescope', __name__)

logger = get_logger('api.telescope')


# Mock dagor_focus early:

def mock():
    logger.warning("*** MOCK MODE ***")
    dagor_track_mocked = MagicMock(**{
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
    return dagor_track_mocked


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            set_mock_var()

if read_mock_var():
    dagor_track = mock()


def device_repr():
    status = dagor_track.get_status()
    return status


@api.route('/', methods=['GET', ])
@check_connectivity
@retry_serial
def resource():
    """

    Telescope / mount device

    ### Hyperlinks

    * [Up](..)
    * [Device state](./state/)
    """
    try:
        status = dagor_track.get_status()
    except Exception:
        raise
    else:
        p_(status)
        time_ago = datetime.utcnow() - datetime.utcfromtimestamp(status['current']['t_now'])
        is_active = time_ago < timedelta(seconds=1)
        status = {
            True: http_status.HTTP_200_OK,
            False: http_status.HTTP_503_SERVICE_UNAVAILABLE,
        }[is_active]
        return {
            'ready': is_active,
        }, status


@api.route('/state/', methods=['GET', 'PUT', 'POST', ])
@check_connectivity
@retry_serial
def state_resource():
    """
    Telescope device status.

    ###Fields

    Writable: `position`<br />
    Read-only: all other fields

    ### Methods

    **PUT**: set new telescope state.<br />
    **POST**: sync current celest coordinates.

    ### Hyperlinks

    * [Up](..)
    """
    if request.method in {'PUT', 'POST', }:
        posted = request.method == 'POST'
        n = request.data['position']
        if posted:
            dagor_track.set_position(n)
        else:
            dagor_track.goto(n)
        return (
            device_repr(),
            http_status.HTTP_200_OK if posted else http_status.HTTP_202_ACCEPTED
        )
    return device_repr()


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
        logger.debug(pformat(dagor_track.mock_calls, indent=4))
        return response

    app.run("0.0.0.0")


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            _run_mocked()
        else:
            _run()
