#!/usr/bin/env python
"""Dagor central CLI interface 0.1

Usage:
  tca configure
  tca get [float] (celest | local | altaz)
  tca get chirality
  tca move to local <HA> <DE> [ce | cw] [quick] [track]
  tca move to altaz <ALT> <AZ> [ce | cw] [quick] [track]
  tca move to celest <RA> <DE> [ce | cw] [quick] [notrack]
  tca move to star <NAME> [ce | cw] [quick] [notrack]
  tca move to cat <NAME> [(from <CATALOG>)] [ce | cw] [quick] [notrack]
  tca move to home
  tca track to celest <RA> <DE> [ce | cw]
  tca track to star <NAME> [ce | cw]
  tca track to cat <NAME> [(from <CATALOG>)] [ce | cw]
  tca track start
  tca sync console
  tca stop
  tca manual
  tca set celest <RA> <DE> [blind]
  tca set star <NAME> [blind]
  tca set cat <NAME> [(from <CATALOG>)] [blind]
  tca lights [0 | 1 | 2 | 3]
  tca focus get
  tca focus set <N>
  tca focus goto <N>
  tca [-h | --help | help]
  tca --version

Commands:
  configure         Configure motor drivers before first use and write configuration to EEPROM and Flash memory.
  get               Display current encoder readout.
                    celest: celestial coordinates (right ascension and declination)
                    local: local coordinates (hour angle and declination)
                    altaz: alt-az coordinates
  move to           move to coordinates precisely
  set               Sync current position with provided coordinates
  sync console      Position sync console, for fine position adjust using arrow keys.

Parameters:
  <HA>              Hour angle, 0 - 24
  <DE>              Declination angle, -90 - 90
  <ALT>             Altitude (elevation) angle, 0 - 24
  <AZ>              Azimuth angle, -90 - 90
  ce | cw           Telescope position, ce = "east", cw = "west"
  <NAME>            Name of a start, capitalized, e.g. Vega

Options:
  -h --help         Show this screen.

"""

from __future__ import division
from os import sys, path
sys.path.append(path.dirname(path.abspath(__file__)))

from time import sleep
from docopt import docopt
import ephem
import math
from common import print_, _wait_for_stop, sign, EnterAbort
from formats import parse_hours, parse_degrees, format_hours, format_degrees
import position as dagor_position
import cat as dagor_catalog
import motors as dagor_motors
import track as dagor_track
import path as dagor_path
import focus as dagor_focus
import lights as dagor_lights
import sys
#from common import exit_

#   docopt    http://docopt.org/
#   astar     https://github.com/elemel/python-astar2


def stepped_move_by(delta_local):
    pass


# Run as CLI client

def _main(args):

    if args['configure']:
        dagor_motors.init()
        dagor_motors._ha.disable()
        dagor_motors._de.disable()
        dagor_motors._ha.configure()
        dagor_motors._de.configure()
        dagor_motors._de.configure_flash()  # order is important when writing to flash (not eeprom)
        dagor_motors._ha.configure_flash()

    if args['get'] and (args['celest'] or args['local'] or args['altaz'] or args['chirality']):
        values = {}
        template = ''
        if args['celest']:
            template = "ra={ra}\nde={de}"
            values = dagor_position.get_celest()
            if not args['float']:
                values = {
                    'ra': format_hours(values['ra']),
                    'de': format_degrees(values['de']),
                }
        elif args['local']:
            template = "ha={ha}\nde={de}"
            values = dagor_position.get_local()
            if not args['float']:
                values = {
                    'ha': format_hours(values['ha']),
                    'de': format_degrees(values['de']),
                }
        elif args['altaz']:
            template = "alt={alt}\naz={az}"
            values = dagor_position.get_altaz()
            if not args['float']:
                values = {
                    'alt': format_degrees(values['alt']),
                    'az': format_degrees(values['az']),
                }
        elif args['chirality']:
            template = "{chirality}"
            values = {'chirality': dagor_position.get_chirality()}
        print template.format(**values)

    if args['move']:
        if args['to']:
            track = False
            celest = False
            chirality = None
            if args['ce']:
                chirality = dagor_position.CHIRAL_E
            if args['cw']:
                chirality = dagor_position.CHIRAL_W
            quick = True if args['quick'] else False
            try:
                local_end = None
                if args['local']:
                    track = True if args['track'] else False
                    local_end = {
                        'ha': parse_hours(args['<HA>']),
                        'de': parse_degrees(args['<DE>']),
                    }
                elif args['altaz']:
                    track = True if args['track'] else False
                    altaz_end = {
                        'alt': parse_degrees(args['<ALT>']),
                        'az': parse_degrees(args['<AZ>']),
                    }
                    local_end = dagor_position.altaz_to_local(altaz_end)
                elif args['celest']:
                    track = False if args['notrack'] else True
                    celest = {
                        'ra': parse_hours(args['<RA>']),
                        'de': parse_degrees(args['<DE>']),
                    }
                elif args['star']:
                    track = False if args['notrack'] else True
                    star = ephem.star(args['<NAME>'])
                    star.compute(dagor_position.tican())
                    celest = {
                        'ra': parse_hours(star.ra / math.pi * 12),
                        'de': parse_degrees(star.dec / math.pi * 180),
                    }
                elif args['cat']:
                    track = False if args['notrack'] else True
                    celest = dagor_catalog.get_celest(args['<NAME>'], args['<CATALOG>'])
                elif args['home']:
                    altaz_end = {
                        'alt': 20,
                        'az': 180,
                    }
                    chirality = dagor_position.CHIRAL_W
                    quick = True
                    local_end = dagor_position.altaz_to_local(altaz_end)
                else:
                    raise AttributeError()
                if local_end or celest:
                    if celest:
                        move_to_local(celest=celest, chirality=chirality, quick=quick, track=track)
                    else:
                        move_to_local(local=local_end, chirality=chirality, quick=quick, track=track)
            except EnterAbort:
                print_("stop")
                dagor_motors.stop()
                _wait_for_stop(dagor_motors._ha, dots=True, skip_dots=3)
                _wait_for_stop(dagor_motors._de, dots=True, skip_dots=3)

    if args['track']:
        if args['to']:
            track = False
            celest = False
            chirality = None
            if args['ce']:
                chirality = dagor_position.CHIRAL_E
            if args['cw']:
                chirality = dagor_position.CHIRAL_W
            try:
                local_end = None
                if args['celest']:
                    track = False if args['notrack'] else True
                    celest = {
                        'ra': parse_hours(args['<RA>']),
                        'de': parse_degrees(args['<DE>']),
                    }
                elif args['star']:
                    track = False if args['notrack'] else True
                    star = ephem.star(args['<NAME>'])
                    star.compute(dagor_position.tican())
                    celest = {
                        'ra': parse_hours(star.ra / math.pi * 12),
                        'de': parse_degrees(star.dec / math.pi * 180),
                    }
                elif args['cat']:
                    track = False if args['notrack'] else True
                    celest = dagor_catalog.get_celest(args['<NAME>'], args['<CATALOG>'])
                else:
                    raise AttributeError()
                internal = dagor_position.celest_to_internal(celest, chirality)
                dagor_track.speed_tracking(internal)
            except EnterAbort:
                print_("stop")
                dagor_motors.stop()
                _wait_for_stop(dagor_motors._ha, dots=True, skip_dots=3)
                _wait_for_stop(dagor_motors._de, dots=True, skip_dots=3)
        elif args['start']:
            dagor_track.speed_tracking()

    if args['sync'] and args['console']:
        dagor_track.sync_console()

    if args['stop']:
        dagor_motors.init()
        dagor_motors.stop()

    if args['manual']:
        dagor_motors.set_manual()

    if args['set']:
        if args['celest']:
            ra = parse_hours(args['<RA>'])
            de = parse_degrees(args['<DE>'])
            dagor_position.set_internal(dagor_position.celest_to_internal({'ra': ra, 'de': de}), args['blind'])
        elif args['star']:
            star = ephem.star(args['<NAME>'])
            star.compute(dagor_position.tican())
            dagor_position.set_internal(dagor_position.celest_to_internal({'ra': star.ra / math.pi * 12, 'de': star.dec / math.pi * 180}), args['blind'])
        elif args['cat']:
            celest = dagor_catalog.get_celest(args['<NAME>'], args['<CATALOG>'])
            dagor_position.set_internal(dagor_position.celest_to_internal(celest), args['blind'])

    if args['focus']:
        if args['get']:
            dagor_focus.get_position()
        elif args['set']:
            dagor_focus.set_position(args['<N>'])
        elif args['goto']:
            dagor_focus.goto(args['<N>'])

    if args['lights']:
        n = None
        if args['0']:
            n = 0
        elif args['1']:
            n = 1
        elif args['2']:
            n = 2
        elif args['3']:
            n = 3
        if n is None:
            print dagor_lights.get_lights()
        else:
            dagor_lights.set_lights(n)


def move_to_local(local=None, celest=None, chirality=None, quick=False, track=False):
    if local is not None and celest is not None:
        raise AttributeError('Use only local or celest coordinates, not both!')
    if local is None and celest is None:
        raise AttributeError('Use either local or celest coordinates!')
    internal_start = dagor_position.get_internal()
    track_correction = -27 if track else 0
    if local:
        internal_end = dagor_position.local_to_internal(local, chirality)
    else:  # celest
        internal_end = dagor_position.celest_to_internal(celest, chirality)
    try:
        path = dagor_path.get_path(internal_start, internal_end)
    except dagor_path.NoPath:
        path = None
        #@TODO argument to force move?
        path = [
            dagor_path.Position(internal_start['ha'], internal_start['de']),
            dagor_path.Position(internal_end['ha'], internal_end['de']),
        ]

    #steps = [(1.3, 3000), (0.1, 1000), (0.0, 300), ]
    steps = [(1, 3000), (0, 300), ]
    if quick:
        steps = [(0, 3000), ]

    print "Moving"
    dagor_motors.init()
    print "path: {}".format(path)
    for position in path[1:]:
        for i, (difference, speed) in enumerate(steps):
            internal_now = dagor_position.get_internal()
            position_delta = {
                'ha': position.ha - internal_now['ha'],
                'de': position.de - internal_now['de'],
            }
            step_delta = position_delta.copy()
            if difference < abs(step_delta['de']):
                step_delta['de'] -= difference * sign(step_delta['de'])
            else:
                step_delta['de'] = None
            if difference < abs(step_delta['ha']) * 15:
                step_delta['ha'] -= difference / 15 * sign(step_delta['ha'])
            else:
                step_delta['ha'] = None
            print "position_delta: {}".format(position_delta)
            #print step_delta
            if step_delta['ha'] is None and step_delta['de'] is None:
                print "no step delta, next"
                continue
            speeds = {
                'ha': speed,
                'de': speed,
            }
            if step_delta['ha'] and step_delta['de']:
                speeds['de'] = min(
                    abs(speeds['de'] * step_delta['de'] / (step_delta['ha'] * 15) / 3.2),
                    abs(speeds['de'])
                )
                speeds['ha'] = min(
                    abs(speeds['ha'] * (step_delta['ha'] * 15) / step_delta['de'] * 3.2),
                    abs(speeds['ha'])
                )
            speeds['ha'] += 0  # track_correction
            print "speeds: {}".format(speeds)
            dagor_motors.move_by(step_delta, speeds)
            sys.stdout.write('moving')
            sys.stdout.flush()
            sleep(0.2)
            while dagor_motors.moving():
                _wait_for_stop(dagor_motors._ha, dots=True, skip_dots=3, enter_abort=True)
                _wait_for_stop(dagor_motors._de, dots=True, skip_dots=3, enter_abort=True)
        if track:
            if celest:
                dagor_track.speed_tracking(internal_end)


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
