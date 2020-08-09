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
from flask_api.renderers import HTMLRenderer

from flask_api.decorators import set_renderers
from mock.mock import MagicMock
from docopt import docopt
from datetime import datetime, timedelta
from flask import Blueprint, request, render_template

from flask_api import FlaskAPI, status as http_status

from tca import track as dagor_track
from tca import position as dagor_position
from tca.api import version
from tca.api.utils import (
    RegexConverter,
    set_mock_var, read_mock_var,
    check_connectivity, retry_serial,)

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
    status['ready'] = get_ready(status)
    return status


def get_ready(status=None):
    if not status:
        try:
            status = dagor_track.get_status()
        except Exception:
            raise
    time_ago = datetime.utcnow() - datetime.utcfromtimestamp(
        status['current']['t_now'])
    is_active = time_ago < timedelta(seconds=1)
    return is_active


@api.route('/', methods=['GET', ])
@check_connectivity
@retry_serial
def resource():
    """

    Telescope / mount device

    ### Hyperlinks

    * [Up](..)
    * [Device state](./state/)
    * [Controller (HTML)](./controller/)
    """
    is_active = get_ready()
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

    ### Fields

    Writable: `config`<br />
    Read-only: `current`

    ### Methods

    #### PUT

    Set new desired telescope state.

    ##### Example payload

    ```
    {
        "ready": true,
        "config": {
            "tracking": true,
            "force": false,
            "chirality": "cw",
            "stop_on_target": false,
            "target_home": false,
            "target_park": false,
            "target_altaz": null,
            "target_celest": {
                "de": -5.43210987,
                "ra": 12.13141516
            },
            "rough": false,
            "target_is_static": false
        }
    }
    ```

    #### POST

    Sync current celest coordinates.

    Only `current.celest` key is writable, other keys are ignored.

    ##### Example payload

    ```
    {
        "current": {
            "celest": {
                "de": 0.0,
                "ra": 12.0
            },
        }
    }
    ```

    ### Hyperlinks

    * [Up](..)
    """
    if request.method == 'POST':
        new_celest = request.data['current']['celest']
        new_internal = dagor_position.celest_to_internal(new_celest)
        dagor_position.set_internal(new_internal)
        return (
            device_repr(),
            http_status.HTTP_202_ACCEPTED
        )
    if request.method == 'PUT':
        new_config = request.data['config']
        dagor_track.set_config(new_config)
        return (
            device_repr(),
            http_status.HTTP_202_ACCEPTED
        )
    return device_repr()


@api.route('/controller/', methods=['GET'])
@set_renderers(HTMLRenderer )
@check_connectivity
@retry_serial
def controller_page():
    return render_template('controller.html')


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
