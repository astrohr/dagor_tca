#!/usr/bin/env python
# coding=utf-8
"""Dagor central CLI interface 0.1.3

Usage:
  tca configure
  tca init
  tca api run
  tca get [float] [human] (celest | local | altaz)
  tca get chirality
  tca goto home [ce] [force]
  tca goto park [ce] [force]
  tca goto home2 [cw] [force]
  tca goto altaz <ALT> <AZ> [ce | cw | cc] [quick | track] [force]
  tca goto local <HA> <DE> [ce | cw | cc] [quick | track] [force]
  tca goto celest <RA> <DE> [ce | cw | cc] [quick] [notrack] [force]
  tca goto stellarium [-] [ce | cw | cc] [quick] [notrack] [force]
  tca goto internal <int_HA> <int_DE> [quick | track] [force]
  tca goto this
  tca goto run
  tca stop
  tca manual
  tca motors [ha | de] status
  tca motors [ha | de] reset
  tca sync console
  tca set celest <RA_DE> [blind]
  tca set celest <RA> <DE> [blind]
  tca set altaz <ALT> <AZ> [ce | cw] [blind]
  tca dome (up | down | open | close | stop)
  tca lights [0 | 1 | 2 | 3]
  tca fans [0 | 1 | 2]
  tca focus get
  tca focus set <N>
  tca focus goto <N>
  tca focus rehome
  tca [-h | --help | help]
  tca --version

Commands:
  init              Re-home focuser and run tracking engine.
  api run           Stat HTTP api, used by ASCOM drivers.
  configure         Configure motor drivers before first use and write
                    configuration to EEPROM and Flash memory.
  get               Display current encoder readout.
                    celest: celestial coordinates
                            (right ascension and declination)
                    local: local coordinates
                           (hour angle and declination)
                    altaz: alt-az coordinates
  move to           move to coordinates precisely
  set               Sync current position with provided coordinates
  sync console      Position sync console, for fine position adjust
                    using arrow keys.
  dome              Control dome rotation and door.
  fans              Control fans on primary mirror.
                    0: stop, 1: half-speed, 2: full-speed
                    empty: get current speed

Parameters:
  <HA>              Hour angle, 0 - 24
  <DE>              Declination angle, -90 - 90
  <ALT>             Altitude (elevation) angle, 0 - 24
  <AZ>              Azimuth angle, -90 - 90
  ce | cw | cc      Chirality, side of pillar when pointing South
                    ce: East
                    cw: West
                    cc: Closest
                    default: keep same chirality
  force             Go directly to specified coordinates,
                    disregarding safety constraints.
                    Use very carefully!

Options:
  -h --help         Show this screen.

"""

from __future__ import division, print_function

from os import sys, path
sys.path.append(path.dirname(path.abspath(__file__)))  # noqa
from docopt import docopt
from local import configuration
from formats import parse_hours, parse_degrees, format_hours, format_degrees
import position as dagor_position
import api
import motors as dagor_motors
import track as dagor_track
import focus as dagor_focus
import fans as dagor_fans
import dome as dagor_dome
import lights as dagor_lights
import sys

# docopt    http://docopt.org/
# astar     https://github.com/elemel/python-astar2


# Run as CLI client

def _main(args):
    only_run = False

    if args['api']:
        if args['run']:
            api.run()
            return

    if args['init']:
        dagor_focus.rehome()
        dagor_track.run()

    if args['configure']:
        dagor_motors.init()
        dagor_motors.configure()
        return

    got_get_arg = any(
        (args['celest'], args['local'], args['altaz'], args['chirality']))
    if args['get'] and got_get_arg:
        values = {}
        template = ''
        if args['celest']:
            if args['human']:
                template = "ra={ra}\nde={de}"
            else:
                template = "{ra} {de}"
            values = dagor_position.get_celest()
            if not args['float']:
                values = {
                    'ra': format_hours(values['ra']),
                    'de': format_degrees(values['de']),
                }
        elif args['local']:
            if args['human']:
                template = "ha={ha}\nde={de}"
            else:
                template = "{ha} {de}"
            values = dagor_position.get_local()
            if not args['float']:
                values = {
                    'ha': format_hours(values['ha']),
                    'de': format_degrees(values['de']),
                }
        elif args['altaz']:
            if args['human']:
                template = "alt={alt}\naz={az}"
            else:
                template = "{alt} {az}"
            values = dagor_position.get_altaz()
            if not args['float']:
                values = {
                    'alt': format_degrees(values['alt']),
                    'az': format_degrees(values['az']),
                }
        elif args['chirality']:
            template = "{chirality}"
            values = {'chirality': dagor_position.get_chirality()}
        print(template.format(**values))
        return

    if args['goto'] and not args['focus']:
        track = False
        stop_on_target = True
        quick = True if args['quick'] else False
        target_celest = None
        chirality = None  # default: None (keep chirality)

        if args['ce']:
            chirality = dagor_position.CHIRAL_E
        if args['cw']:
            chirality = dagor_position.CHIRAL_W
        if args['cc']:
            chirality = dagor_position.CHIRAL_CLOSEST

        if args['home']:
            chirality = dagor_position.HOME_CHIRALITY
            if args['ce']:
                chirality = dagor_position.CHIRAL_E
            target_celest = dagor_position.altaz_to_celest(
                dagor_position.HOME_ALTAZ)
            quick = True
            stop_on_target = True

        if args['home2']:
            chirality = dagor_position.HOME_N_CHIRALITY
            if args['cw']:
                chirality = dagor_position.CHIRAL_W
            target_celest = dagor_position.altaz_to_celest(
                dagor_position.HOME_N_ALTAZ)
            quick = True
            stop_on_target = True

        if args['park']:
            chirality = dagor_position.PARK_CHIRALITY
            if args['ce']:
                chirality = dagor_position.CHIRAL_E
            target_celest = dagor_position.altaz_to_celest(
                dagor_position.PARK_ALTAZ)
            quick = True
            stop_on_target = True

        elif args['altaz']:
            track = True if args['track'] else False
            stop_on_target = not track
            altaz_end = {
                'alt': parse_degrees(args['<ALT>']),
                'az': parse_degrees(args['<AZ>']),
            }
            if chirality is None:
                chirality = dagor_position.CHIRAL_CLOSEST
            target_celest = dagor_position.altaz_to_celest(altaz_end)

        elif args['local']:
            track = True if args['track'] else False
            stop_on_target = not track
            local_end = {
                'ha': parse_hours(args['<HA>']),
                'de': parse_degrees(args['<DE>']),
            }
            target_celest = dagor_position.local_to_celest(local_end)

        elif args['celest'] or args['stellarium']:

            if args['celest']:
                target_celest = {
                    'ra': parse_hours(args['<RA>']),
                    'de': parse_degrees(args['<DE>']),
                }

            elif args['stellarium']:
                # read stdin:
                stellarium_ra_dec = None
                target_prefix = "RA/Dec ({}): ".format(
                    configuration.TRACKING["stellarium_mode"])
                print("Paste object info from"
                      " Stellarium then press Enter twice:")
                while True:
                    input_ = raw_input().strip()
                    if input_ == '':
                        break
                    if input_.startswith(target_prefix):
                        stellarium_ra_dec = input_[len(target_prefix):]
                if not stellarium_ra_dec:
                    sys.stderr.write(
                        'No line starts with "{}" in the pasted data.\n'
                        .format(target_prefix))
                    exit(1)
                stellarium_ra, stellarium_de = stellarium_ra_dec.split('/', 1)
                target_celest = {
                    'ra': parse_hours(stellarium_ra),
                    'de': parse_degrees(stellarium_de),
                }

            track = False if args['notrack'] else True
            stop_on_target = not track

        elif args['internal']:
            internal_end = {
                'ha': args['<int_HA>'],
                'de': args['<int_DE>'],
            }
            target_celest = dagor_position.internal_to_celest(internal_end)
            track = True if args['track'] else False
            stop_on_target = True

        elif args['this']:
            # stat tracking current coordinates
            track = True
            stop_on_target = False
            quick = False
            target_celest = None

        elif args['run']:
            # stat tracking current coordinates
            only_run = True

        # start track console:
        if only_run:
            dagor_track.run()
        else:
            dagor_track.speed_tracking(
                target_celest,
                chirality=chirality,
                target_is_static=not track,
                stop_on_target=stop_on_target,
                rough=quick,
                force=args['force'],
            )
        return

    if args['sync'] and args['console']:
        dagor_track.sync_console()
        return

    if args['stop']:
        dagor_motors.init()
        dagor_motors.stop()
        return

    if args['manual']:
        dagor_motors.set_manual()
        return

    if args['set']:
        if args['celest']:
            if args['<RA_DE>']:
                rade = args['<RA_DE>']
                try:
                    rade = rade.replace('RA ', '').replace('  Dec ', '')
                    arg_ra, arg_de = rade.split(',')
                except Exception:
                    raise ValueError('Cannot parse format')
            else:
                arg_ra = args['<RA>']
                arg_de = args['<DE>']
            ra = parse_hours(arg_ra)
            de = parse_degrees(arg_de)
            dagor_position.set_internal(
                dagor_position.celest_to_internal(
                    {'ra': ra, 'de': de}),
                args['blind'])

        if args['altaz']:
            arg_alt = args['<ALT>']
            arg_az = args['<AZ>']
            alt = parse_degrees(arg_alt)
            az = parse_degrees(arg_az)
            chirality = None
            if args['ce']:
                chirality = dagor_position.CHIRAL_E
            if args['cw']:
                chirality = dagor_position.CHIRAL_W

            dagor_position.set_internal(
                dagor_position.altaz_to_internal(
                    {'alt': alt, 'az': az},
                    chirality
                ),
                args['blind'])
        return

    if args['focus']:
        if args['get']:
            print(dagor_focus.get_position())
        elif args['set']:
            dagor_focus.set_position(args['<N>'])
        elif args['goto']:
            dagor_focus.goto(args['<N>'])
        elif args['rehome']:
            dagor_focus.rehome()
        return

    if args['motors']:
        if args['reset']:
            dagor_motors.reset(args['ha'], args['de'])
        if args['status']:
            dagor_motors.status_str(args['ha'] or None, args['de'] or None)
        return

    if args['dome']:
        if args['up']:
            dagor_dome.dome_up()
        if args['down']:
            dagor_dome.dome_down()
        if args['stop']:
            dagor_dome.dome_stop()
        if args['open']:
            dagor_dome.dome_open()
        if args['close']:
            dagor_dome.dome_close()
        return

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
            print(dagor_lights.get_lights())
        else:
            dagor_lights.set_lights(n)
        return

    if args['fans']:
        n = None
        if args['0']:
            n = 0
        elif args['1']:
            n = 1
        elif args['2']:
            n = 2
        if n is None:
            print(dagor_fans.get_fan(1))
        else:
            dagor_fans.set_fan(1, n)
        return

if __name__ == '__main__':

    cli_args = docopt(
        __doc__, version=__doc__.split("\n")[0], options_first=True)

    if len(sys.argv) == 1 or cli_args['help']:
        print(__doc__.strip())
        exit(0)

    _main(cli_args)
