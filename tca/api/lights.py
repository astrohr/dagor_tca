#!/usr/bin/env python
# coding=utf-8

"""Dagor lights API 0.2.

Usage:
    lights.py run [-m]
    lights.py -h | --help
    lights.py --version

Commands:
    run            Start the API server.

Options:
    -m --mock      Mock any hardware devices, useful for testing and development.
    -h --help      Show this screen or description of specific command.
    --version      Show version.
"""

from functools import wraps
from mock.mock import MagicMock
from pprint import pprint

from docopt import docopt
from flask import Blueprint, request, make_response
from flask.ext.api.decorators import set_renderers, set_parsers
from flask_api import FlaskAPI, status
from flask_api.exceptions import ParseError

from tca import lights as dagor_lights
from tca.api.utils import RegexConverter, BoolRenderer, BoolBrowsableAPIRenderer, \
    BoolParser, render_error

DEFAULT_PREFIX = '/lights'

api = Blueprint('lights', __name__)


# Mock dagor_lights early:
if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            print("*** MOCK MODE ***")
            dagor_lights = MagicMock(**{
                'get_lights.return_value': 2,
                'get_light.side_effect': lambda n: n == 1,
            })


def device_repr():
    return {
        'n': dagor_lights.get_lights(),
    }


def light_repr(n):
    return "true" if dagor_lights.get_light(n) else "false"


# decorators:


def check_connectivity(func):
    @wraps(func)
    def func_wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except dagor_lights.CommunicationException as e:
            return {
                'ready': False,
                'message': '{}'.format(e),
            }, status.HTTP_503_SERVICE_UNAVAILABLE
    return func_wrapper


def handle_request_errors(func):
    @wraps(func)
    def func_wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except ParseError as e:
            return render_error(e)
    return func_wrapper



@api.route('/', methods=['GET', ])
@check_connectivity
def resource():
    """
    Hyperlinks:

    * [Device state](./state/)
    * [Light 1](./1/)
    * [Light 2](./2/)
    """
    try:
        dagor_lights.get_lights()
    except:
        raise
    else:
        return {
            'ready': True,
        }, status.HTTP_200_OK


@api.route('/state/', methods=['GET', 'PUT', 'POST', ])
@check_connectivity
def state_resource():
    """
    State for all light (two of them).

    * 0: all OFF
    * 1: light 1 ON, light 2 OFF
    * 2: light 1 OFF, light 2 ON
    * 3: all ON

    It's binary...

    Hyperlinks:

    * [Up](..)
    """
    if request.method in {'PUT', 'POST', }:
        n = request.data['n']
        dagor_lights.set_lights(n)
        return device_repr(), status.HTTP_202_ACCEPTED
    return device_repr()


@api.route('/<regex("[0-9]+"):light_i>/', methods=['GET', 'PUT', 'POST', ])
@set_renderers(BoolBrowsableAPIRenderer, BoolRenderer)
@set_parsers(BoolParser)
@check_connectivity
@handle_request_errors
def light_resource(light_i):
    """
    State for a single light. It is either ON or OFF (case insensitive).

    Hyperlinks:

     * [Up](..)
    """
    light_i = int(light_i)  # URL regex guarantees digits only
    if request.method in {'PUT', 'POST', }:
        # is body is empty, parsers never get called, so:
        if not request.content_length:
            raise ParseError('Empty request, use "true" or "false".')
        # get data, via parser:
        data = request.data
        dagor_lights.set_light(light_i, data)
    response = make_response(light_repr(light_i - 1))
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
        # noinspection PyProtectedMember
        pprint(dagor_lights._mock_mock_calls, indent=4)
        return response

    app.run("0.0.0.0")


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            _run_mocked()
        else:
            _run()

