#!/usr/bin/env python
# coding=utf-8

"""Dagor Fans API {VERSION}

Usage:
    fans.py run [-m]
    fans.py -h | --help
    fans.py --version

Commands:
    run          Start the API server.

Options:
    -m --mock    Mock any hardware devices, useful for testing and development.
    -h --help    Show this screen or description of specific command.
    --version    Show version.
"""
from collections import OrderedDict
from functools import wraps
from mock.mock import MagicMock
from docopt import docopt
from flask import Blueprint, request, make_response
from flask.ext.api.decorators import set_renderers, set_parsers
from flask_api import FlaskAPI, status as http_status
from flask_api.exceptions import ParseError

from tca import fans as dagor_fans

from tca.api import version
from tca.api.utils import (
    RegexConverter, render_error,
    set_mock_var, read_mock_var, IntBrowsableAPIRenderer, IntParser,
    IntRenderer)
from tca.logging_conf import get_logger

# noinspection PyUnboundLocalVariable
__doc__ = __doc__.format(VERSION=version)


DEFAULT_PREFIX = '/fans'
api = Blueprint('fans', __name__)
logger = get_logger('api.fans')


# Mock dagor_fans early:

# TO-DO: Update mock interface
def mock():
    logger.warning("*** MOCK MODE ***")
    dagor_fans_mocked = MagicMock(**{
        'get_fans.return_value': 2,
        'get_fans.side_effect': lambda n: n == 1,
    })
    return dagor_fans_mocked


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            set_mock_var()


if read_mock_var():
    dagor_lights = mock()


def fan_repr(n):
    return dagor_fans.get_fan(n)


def device_repr():
    status = dagor_fans.get_fans()
    response = OrderedDict()
    response["fan 1"] = int(status[0])
    response["fan 2"] = int(status[1])
    return response


# decorators:

def check_connectivity(func):
    @wraps(func)
    def func_wrapper(*args_, **kwargs_):
        try:
            return func(*args_, **kwargs_)
        except dagor_fans.CommunicationException as e:
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


@api.route('/state/', methods=['GET', 'PUT', ])
@check_connectivity
def state_resource():
    """
    Fans device status.

    ### Hyperlinks

    * [Up](..)
    """
    if request.method in {'PUT', }:
        dagor_fans.set_fan(1, request.data["fan 1"])
        dagor_fans.set_fan(2, request.data["fan 2"])
    return device_repr()


@api.route('/', methods=['GET', ])
@check_connectivity
def resource():
    """
    Control for the Central Fan (1) and three other fans (2) on the telescope.

    ### Hyperlinks:

    * [Up](..)
    * [Device state](./state/)
    * [Central fan](./1/)
    * [Other three fans](./2/)
    """
    try:
        dagor_fans.get_fans()
    except:
        raise
    else:
        return {
            'ready': True,
        }, http_status.HTTP_200_OK


@api.route('/<regex(1|2):fan_i>/', methods=['GET', 'PUT', ])
@set_renderers(IntBrowsableAPIRenderer, IntRenderer)
@set_parsers(IntParser)
@check_connectivity
@handle_request_errors
def fan_resource(fan_i):
    """
    State for a Central Fan (1) or other three fans (2).
    In both cases there are three possible states:

        0 -> OFF
        1 -> Speed 1
        2 -> Speed 2

    ### Hyperlinks:

     * [Up](..)
    """

    fan_i = int(fan_i)  # URL regex guarantees digits only
    if request.method in {'PUT', 'POST', }:
        # is body is empty, parsers never get called, so:
        if not request.content_length:
            raise ParseError('Empty request, use "0", "1" or "2".')
        # get data, via parser:
        data = request.data
        dagor_fans.set_fan(fan_i, data)
    response = make_response(str(fan_repr(fan_i)))
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
        logger.debug(pformat(dagor_lights.mock_calls, indent=4))
        return response

    app.run("0.0.0.0")


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            _run_mocked()
        else:
            _run()
