#!/usr/bin/env python
"""Dagor star catalog 0.2

Usage:
  cat.py get [float] <OBJECT> [(from <CATALOG>)] [as (celest | local | altaz)]
  cat.py dump all [(from <CATALOG>)] [in (json)]
  cat.py [-h | --help | help]
  cat.py --version

Commands:
  get               Display coordinates of an object.
                    float: display coordinates as decimal numbers (hours for ra, degrees for dec)

Options:
  -h --help         Show this screen.

"""
import sys
import os
from docopt import docopt
import ephem
import math
from formats import format_hours, format_degrees
from common import BASE_PATH
import position as dagor_position


def get_entry(name, catalog_name=None):
    if not catalog_name:
        catalog_name = 'default'
    with open(os.path.join(BASE_PATH, 'catalogs/%s.edb' % catalog_name)) as f:
        lines = f.read().splitlines()
    if name[-1] not in (',', ' '):
        name1 = name + ','
        name2 = name + ' '
    for line in lines:
        if line[:len(name1)] == name1:
            return line
    for line in lines:
        if line[:len(name2)] == name2:
            return line
    raise ValueError(u'No object named {} in catalog {}.'.format(name, catalog_name))


def get_celest(name, catalog_name=None):
    line = get_entry(name, catalog_name)
    obj = ephem.readdb(line)
    obj.compute(dagor_position.tican())
    return {
        'ra': float(obj._ra) / math.pi * 12,
        'de': float(obj._dec) / math.pi * 180
    }


def get_all_objects(catalog_name=None):
    if not catalog_name:
        catalog_name = 'default'
    with open(os.path.join(BASE_PATH, 'catalogs/%s.edb' % catalog_name)) as f:
        lines = f.read().splitlines()
    for line in lines:
        try:
            obj = ephem.readdb(line)
            obj.compute(dagor_position.tican())
            yield obj
        except ValueError:
            pass


def dump(catalog_name=None):
    retval = []
    for obj in get_all_objects(catalog_name):
        celest = {
            'ra': obj._ra,
            'de': obj._dec,
        }
        local = dagor_position.celest_to_local(celest)
        altaz = dagor_position.celest_to_altaz(celest)
        retval.append({
            'name': obj.name,
            'coordinates': {
                'celest': celest,
                'local': local,
                'altaz': altaz,
            }
        })
    return retval


# Run as CLI client

def _main(args):
    if args['get']:
        name = args['<OBJECT>']
        system = 'celest'
        if args['local']:
            system = 'local'
        if args['altaz']:
            system = 'altaz'
        celest = get_celest(args['<OBJECT>'], args['<CATALOG>'])
        if system == 'celest':
            if not args['float']:
                print format_hours(celest['ra'])
                print format_degrees(celest['de'])
            else:
                print celest['ra']
                print celest['de']
        elif system == 'local':
            local = dagor_position.celest_to_local(celest)
            if not args['float']:
                print format_hours(local['ha'])
                print format_degrees(local['de'])
            else:
                print local['ha']
                print local['de']
        elif system == 'altaz':
            altaz = dagor_position.celest_to_altaz(celest)
            if not args['float']:
                print format_degrees(altaz['alt'])
                print format_degrees(altaz['az'])
            else:
                print altaz['alt']
                print altaz['az']
    if args['dump']:
        if args['all']:
            print dump()

if __name__ == '__main__':

    args = docopt(__doc__, version=__doc__.split("\n")[0], options_first=True)

    if len(sys.argv) == 1 or args['help']:
        print __doc__.strip()
        exit(0)

    try:
        _main(args)
    except:
        raise
        #exit_('ERROR')


