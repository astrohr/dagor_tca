#!/usr/bin/env python
# coding=utf-8

"""Dagor API {VERSION}

Usage:
    api run
    api -h | --help
    api --version

Commands:
    run            Start the API server.

Options:
    -m --mock      Mock any hardware devices, useful for testing and development.
    -h --help      Show this screen or description of specific command.
    --version      Show version.
"""
from tca.api import version
__doc__ = __doc__.format(VERSION=version)

from docopt import docopt
from flask_api import FlaskAPI
import lights
import focus
from tca.api.utils import RegexConverter

app = FlaskAPI(__name__)
app.debug = False
app.url_map.converters['regex'] = RegexConverter

for module in (lights, focus, ):
    app.register_blueprint(module.api, url_prefix=module.DEFAULT_PREFIX)


@app.route('/')
def hello_world():
    return 'Dagor API {}'.format(version)


def _run():
    app.run("0.0.0.0", 8001)


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        _run()

