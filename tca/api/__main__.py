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
import sys
from docopt import docopt
from api import version, run

# noinspection PyUnboundLocalVariable
__doc__ = __doc__.format(VERSION=version)


# mock early, before first import:
if __name__ == '__main__':
    # noinspection PyUnboundLocalVariable
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        if args['--mock']:
            os.environ['DAGOR_API_MOCK'] = '1'


if __name__ == '__main__':
    API_DIR = os.path.dirname(__file__)
    BASE_DIR = os.path.dirname(API_DIR)
    sys.path.append(BASE_DIR)
    args = docopt(__doc__, version=__doc__.strip().split('\n')[0])
    if args['run']:
        run()
