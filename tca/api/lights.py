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

import sys
from functools import wraps
from urlparse import parse_qs
from docopt import docopt
from flask import Blueprint, request, make_response
from flask_api import FlaskAPI, status
from flask_api.exceptions import ParseError
from mock.mock import patch, MagicMock

from tca import lights as dagor_lights

DEFAULT_PREFIX = '/lights'

api = Blueprint('lights', __name__)


# TODO: use custom parser instead! FlaskApi supports it.
def fix_browsable_api_content(data):
    if "_content_type=" in data:
        query_params = parse_qs(data)
        print(query_params)
        try:
            data = query_params['_content'][0]
        except IndexError:
            data = None
    return data


def device_repr():
    return {
        'n': dagor_lights.get_lights(),
    }


def light_repr(n):
    return "true" if dagor_lights.get_light(n) else "false"


def parse_light_repr(repr):
    try:
        if repr.lower().strip() == "true":
            return True
        if repr.lower().strip() == "false":
            return False
        raise ValueError
    except ValueError:
        raise ParseError('invalid representation, use "true" or "false"')


def check_connectivity(func):
    @wraps(func)
    def func_wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except dagor_lights.CommunicationException as e:
            return {
                'ready': False,
                'error': '{}'.format(e)
            }, status.HTTP_503_SERVICE_UNAVAILABLE
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


@api.route('/state/', methods=['GET', 'PUT'])
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
    if request.method in {'PUT', }:
        n = request.data['n']
        dagor_lights.set_lights(n)
        return device_repr(), status.HTTP_202_ACCEPTED
    return device_repr()


@api.route('/<light_i>/', methods=['GET', 'POST', ])
@check_connectivity
def light_resource(light_i):
    """
    State for a single light. It is either ON or OFF (case insensitive).

    Hyperlinks:

     * [Up](..)
    """
    try:
        light_i = int(light_i)
        if request.method in {'POST', }:
            data = request.get_data()
            print(data)
            data = fix_browsable_api_content(data)
            print(data)
            value = parse_light_repr(data)
            dagor_lights.set_light(light_i, value)
        response = make_response(light_repr(light_i - 1))
        return response
    except ValueError as e:
        raise ParseError(str(e))


def _run():
    app = FlaskAPI(__name__)
    app.debug = True
    app.register_blueprint(api, url_prefix=DEFAULT_PREFIX)
    app.run("0.0.0.0")


#@patch('tca.lights')
def _run_mocked():
    raise NotImplementedError()
    dagor_lights = MagicMock()
    _run()
    print(dagor_lights.__dict__)


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.split('\n'[0]))

    if args['--version']:
        sys.exit(0)

    if args['run']:
        if args['--mock']:
            _run_mocked()
        else:
            _run()

