#!/usr/bin/env python
# coding=utf-8

"""Dagor API {VERSION}

Usage:
    api run [-m]
    api -h | --help
    api --version

Commands:
    run          Start the API server.

Options:
    -m --mock    Mock any hardware devices, useful for testing and development.
    -h --help    Show this screen or description of specific command.
    --version    Show version.
"""
import os
from docopt import docopt
from collections import OrderedDict
from flask.ext.api.decorators import set_renderers
from flask.ext.api.renderers import JSONRenderer
from flask_api import FlaskAPI

# mock early, before first import:
if __name__ == '__main__':
    # noinspection PyUnboundLocalVariable
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            os.environ['DAGOR_API_MOCK'] = '1'

from tca.api import version, MODULES
from tca.api.utils import RegexConverter, BrowsableAPITitleRenderer

# noinspection PyUnboundLocalVariable
__doc__ = __doc__.format(VERSION=version)

app = FlaskAPI(__name__)
app.debug = False
app.url_map.converters['regex'] = RegexConverter

for module in MODULES:
    app.register_blueprint(module.api, url_prefix=module.DEFAULT_PREFIX)


@set_renderers(BrowsableAPITitleRenderer, JSONRenderer)
def dagor_api():
    """
    API root

    version: {VERSION}

    ### Hyperlinks

    {modules}

    """
    value = OrderedDict()
    value['version'] = version
    return value


modules = []
for module in MODULES:
    modules.append(
        ' * [{name}]({url}/)'.format(
            name=module.api.name,
            url=module.DEFAULT_PREFIX,
        )
    )
dagor_api.__doc__ = dagor_api.__doc__.format(
    VERSION=version,
    modules='\n    '.join(modules)
)
dagor_api.__name__ = "Dagor API"
app.route('/')(dagor_api)


def _run():
    app.run("0.0.0.0", 8001)


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        _run()
