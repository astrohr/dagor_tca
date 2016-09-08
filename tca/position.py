#!/usr/bin/env python
"""Dagor position readout and calculator 0.3

Usage:
  position.py get raw
  position.py get normalised
  position.py get internal
  position.py get ring
  position.py get local
  position.py get celest
  position.py get altaz
  position.py get chirality
  position.py set internal <HA> <DE>
  position.py set celest <RA> <DE> [blind]
  position.py set star <NAME> [blind]
  position.py set cat <NAME> [(from <CATALOG>)] [blind]
  position.py [-h | --help | help]
  position.py --version

Commands:
  get               Display current encoder readout.
                    raw: directly from encoders
                    normalised: same as raw, but with HA normalised to avoid overflow problems
                    internal: angular coordinates internal to Dagor
                    ring: internal coordinates, but DE rendered to show sector and angle [0, 90]
                    local: local coordinates (hour angle and declination)
                    celest: celestial coordinates (right ascension and declination)
                    altaz: alt-az coordinates
  set               Set current telescope position to specified coordinates.
                    Useful for correcting coordinates when an objects is targeted (and tracked)

Options:
  -h --help         Show this screen.

"""

from __future__ import division
import os
import sys
from docopt import docopt
import ephem
import math
import cat as dagor_catalog
#from common import exit_

#   docopt    http://docopt.org/
#   astar     https://github.com/elemel/python-astar2
from common import BASE_PATH
from formats import format_hours, format_degrees


LATITUDE = 45.290871
LONGITUDE = 13.749793
ALTITUDE = 330


def tican():
    observer = ephem.Observer()
    observer.lat = math.radians(LATITUDE)
    observer.lon = math.radians(LONGITUDE)
    observer.elevation = ALTITUDE
    return observer


_ENCODERS_DATA_FILE = '/var/dagor/run/position.txt'
POSITION_CORRECTIONS_FILE = os.path.join(BASE_PATH, 'position_corrections.txt')

_ENCODER_MAX = 8388607
_HA_BLIND_POINT = 5000000
_DE_BLIND_POINT = 6500000

_HA_INTERNAL_BLIND_POINT = 5  # Approximately corresponds to above blind points. Very very approximately!
_DE_INTERNAL_BLIND_POINT = 0  # Approximately corresponds to above blind points. Very very approximately!

_HA_SLOPE = -349593.3017543860
_HA_INTERSECTION = 7445177.90526316

_DE_SLOPE = -23300.2286699885
_DE_INTERSECTION = 6543059.76197777

_HA_REAL_OFFSET = -0.0314481764 - -0.0045467897 - 0.0045420766 + 0.046 - 0.0854678 - 0.035 - 0.024444944100000754
_DE_REAL_OFFSET = 279.44431524230004 + 0.040461855 + -0.0156058897 + 0.0023 + 0.0194028 + 0.5 + 0.013929050277777577

CHIRAL_E = "ce"
CHIRAL_W = "cw"
CHIRAL_CLOSEST = 'cc'

CHIRAL_DE_BOUNDARY = 189.9909042

HOME_ALTAZ = {
    'alt': 15,
    'az': 172,
}
HOME_CHIRALITY = CHIRAL_W

HOME_N_ALTAZ = {
    'alt': 15,
    'az': 0,
}
HOME_N_CHIRALITY = CHIRAL_E




def read_internal_correction_file():
    corrections = {
        'ha': 0,
        'de': 0,
    }
    try:
        with open(POSITION_CORRECTIONS_FILE) as f:
            lines = [line for line in f.readlines() if line[0] != '#']
    except IOError:
        lines = [0, 0]
    if len(lines) > 1:
        corrections['ha'] = float(lines[0])
        corrections['de'] = float(lines[1])
    return corrections


def reset_correction_file():
    write_correction_file({'ha': 0, 'de': 0})


def write_correction_file(corrections):
    with open(POSITION_CORRECTIONS_FILE) as f:
        lines = [line if line[0] == '#' else '{}\n' for line in f.readlines()]
    with open(POSITION_CORRECTIONS_FILE, 'w') as f:
        f.write(''.join(lines).format(corrections['ha'], corrections['de']))


def set_internal(internal_new, blind=False):
    internal_old = get_internal()
    existing_corrections = read_internal_correction_file()
    new_delta = {
        'ha':  internal_new['ha'] - internal_old['ha'],
        'de':  internal_new['de'] - internal_old['de'],
    }
    print "Existing correction:"
    print format_hours(existing_corrections['ha']), format_degrees(existing_corrections['de'])
    print "New delta:"
    print format_hours(new_delta['ha']), format_degrees(new_delta['de'])
    print
    if not blind:
        write_correction_file({
            'ha': new_delta['ha'] + existing_corrections['ha'],
            'de': new_delta['de'] + existing_corrections['de'],
        })


def _read():
    """
    Access to IK220 driver is done via C daemon. The daemon writes data to file, this function simply reads the file.
    Control the daemon:
        $ service dagor_ik220_run start
        $ service dagor_ik220_run stop
    File:
        see _ENCODERS_DATA_FILE
    """
    with open(_ENCODERS_DATA_FILE) as f:
        content = f.readlines()
    ha_raw = float(content[1])
    de_raw = float(content[2])
    return {
        'ha_raw': ha_raw,
        'de_raw': de_raw,
    }


def get_raw():
    """
    As on the encoders.
    Can overflow! Use get_normalised() instead.
    """
    data = _read()
    return {
        'ha': data['ha_raw'],
        'de': data['de_raw'],
    }


def get_normalised():
    """
    This is solely to evade encoder counter overflow (it's simpler to deal with a continuous affine function).
    """
    data = _read()
    return {
        'ha': data['ha_raw'] - _ENCODER_MAX if data['ha_raw'] > _HA_BLIND_POINT else data['ha_raw'],
        'de': data['de_raw'] - _ENCODER_MAX if data['de_raw'] > _DE_BLIND_POINT else data['de_raw'],
    }


def get_internal():
    """
     Calculates internal position of the telescope (as defined on the rings.
     HA is measured in hours, interval [7, 28]
     DE is measured in degrees, interval [50, 340] (no sectors)
    """
    data = get_normalised()
    corrections = read_internal_correction_file()
    return {
        'ha': (data['ha'] - _HA_INTERSECTION) / _HA_SLOPE + corrections['ha'],
        'de': (data['de'] - _DE_INTERSECTION) / _DE_SLOPE + corrections['de']
    }


def get_ring(data=None):
    """
     Same as "internal" except for DE axes which is represented with angle [0, 90] and a sector number.
    """
    if data is None:
        data = get_internal()
    de = data['de']
    de_sector = 1 if 0 <= de < 90 else \
                2 if 90 <= de < 180 else \
                3 if 180 <= de < 270 else \
                4 if 270 <= de <= 360 else None
    if de_sector is None:
        raise ValueError('DE not in range [0, 360]')
    de_ring = {
        1: de,
        2: 180 - de,
        3: de - 180,
        4: 360 - de,
    }[de_sector]
    return {
        'ha': data['ha'],
        'de': (de_ring, de_sector),
    }


def _gauge_ha(ha_internal):
    """
    Helper, don't rely on.
    Chirality - unaware!
    """
    ha_local = ha_internal + _HA_REAL_OFFSET
    if ha_local > 24:
        ha_local -= 24
    if ha_local < 0:
        ha_local += 24
    return ha_local


def _gauge_de(de_internal):
    """
    Helper, don't rely on.
    Chirality - unaware!
    """
    de_local = -de_internal + _DE_REAL_OFFSET
    if de_local > 90:
        de_local = 180 - de_local
    if de_local < -90:
        de_local = 180 + de_local
    return de_local


def _degauge_ha(ha_local):
    """
    Helper, don't rely on.
    Chirality - unaware!
    """
    ha_internal = ha_local - _HA_REAL_OFFSET
    if ha_internal < _HA_INTERNAL_BLIND_POINT:
        ha_internal += 24
    return ha_internal


def _degauge_de(de_local):
    """
    Helper, don't rely on.
    Chirality - unaware!
    """
    de_internal = -de_local + _DE_REAL_OFFSET
    return de_internal


def _get_chirality(de_internal):
    if CHIRAL_DE_BOUNDARY - 180 < de_internal < CHIRAL_DE_BOUNDARY:
        return CHIRAL_E
    else:
        return CHIRAL_W


def get_chirality(internal_de=None):
    if internal_de is None:
        internal_de = get_internal()['de']
    else:
        try:
            internal_de = internal_de['de']
        except TypeError:
            pass
    return _get_chirality(internal_de)


def get_local():
    """
    Telescope local coordinates
        {'ha': HA, 'de': DE}
        HA - hour angle, float in range [0,24]
        DE - declination, float in range [-90, 90]
    """
    return internal_to_local(get_internal())


def get_celest():
    """
    Pure celestial coordinates
        {'ra': RA, 'de': DE}
         RA - right ascension, float in range [0, 24]
         DE - declination, float in range [-90, 90]
    """
    return local_to_celest(get_local())


def get_altaz():
    """
    Altitude and Azimuth
    """
    return celest_to_altaz(get_celest())


# Helpers

def angular_distance(internal_0, internal_1):
    dist_ha = (internal_0['ha'] - internal_1['ha']) * 15
    dist_de = internal_0['de'] - internal_1['de']
    distance = (dist_ha ** 2 + dist_de ** 2) ** (1/2)
    return distance


# Coordinate conversions

def internal_to_local(internal):
    """
        {'ha': float, 'de': float}  -->  {'ha': float, 'de': float}
    """
    ha = _gauge_ha(internal['ha'])
    de = _gauge_de(internal['de'])
    ch = _get_chirality(internal['de'])
    if ch == CHIRAL_E:
        de = 180 - de
        if de > 0:
            de = 180 - de
        else:
            de = -180 - de
        ha -= 12
        if ha < 0:
            ha += 24

    return {
        'ha': ha,
        'de': de,
    }


def get_closest_chirality(local):
    current_internal = get_internal()
    distance_w = angular_distance(current_internal, local_to_internal(local, CHIRAL_W))
    distance_e = angular_distance(current_internal, local_to_internal(local, CHIRAL_E))
    if distance_w <= distance_e:
        return CHIRAL_W
    else:
        return CHIRAL_E


def local_to_internal(local, chirality=None):
    if chirality is None:
        chirality = get_chirality()
    elif chirality == CHIRAL_CLOSEST:
        chirality = get_closest_chirality(local)
    ha = local['ha']
    de = local['de']
    if not 0 <= ha <= 24:
        raise ValueError('ha value is not in range [0, 24]')
    if not -90 <= de <= 90:
        raise ValueError('de value is not in range [-90, 90]')
    if chirality == CHIRAL_E:
        ha += 12
        if ha > 24:
            ha -= 24
        de = 180 - de
    ha = _degauge_ha(ha)
    de = _degauge_de(de)
    return {
        'ha': ha,
        'de': de,
    }


def local_to_celest(local):
    de = local['de']
    ra = tican().sidereal_time() * 12 / math.pi - local['ha']
    ra = ra + 24 if ra < 0 else ra - 24 if ra >= 24 else ra
    return {
        'ra': ra,
        'de': de,
    }


def celest_to_local(celest):
    de = celest['de']
    ha = tican().sidereal_time() * 12 / math.pi - celest['ra']
    ha = ha + 24 if ha < 0 else ha - 24 if ha >= 24 else ha
    return {
        'ha': ha,
        'de': de,
    }


def celest_to_internal(celest, chirality=None):
    return local_to_internal(celest_to_local(celest), chirality)


def celest_to_altaz(celest):
    star = ephem.FixedBody()
    star._ra = math.radians(celest['ra'] * 360 / 24)
    star._dec = math.radians(celest['de'])
    star.compute(tican())
    return {
        'alt': float(star.alt) / math.pi * 180,
        'az': float(star.az) / math.pi * 180,
    }


def altaz_to_celest(altaz):
    #@TODO: radec_of() does not account for nutation and aberration, see http://stackoverflow.com/questions/11970713/pyephem-conversion-of-alt-az-to-ra-dec-and-back-not-internally-consistent
    coords = tican().radec_of(altaz['az'] / 180 * math.pi, altaz['alt'] / 180 * math.pi)
    return {
        'ra': float(coords[0]) / math.pi * 12,
        'de': float(coords[1]) / math.pi * 180,
    }


def local_to_altaz(local):
    return celest_to_altaz(local_to_celest(local))


def altaz_to_local(altaz):
    return celest_to_local(altaz_to_celest(altaz))


def internal_to_altaz(internal):
    return local_to_altaz(altaz_to_local(internal))


def altaz_to_internal(altaz, chirality=None):
    return local_to_internal(altaz_to_local(altaz), chirality)


def internal_to_celest(internal):
    return local_to_celest(internal_to_local(internal))


# Run as CLI client

def _main(args):

    if args['get']:
        values = {}
        template = "ha={ha}\nde={de}"
        #@TODO angle formats
        if args['raw']:
            values = get_raw()
        elif args['normalised']:
            values = get_normalised()
        elif args['internal']:
            values = get_internal()
        elif args['ring']:
            values = get_ring()
        elif args['local']:
            values = get_local()
        elif args['celest']:
            values = get_celest()
            template = "ra={ra}\nde={de}"
        elif args['altaz']:
            values = get_altaz()
            template = "alt={alt}\naz={az}"
        elif args['chirality']:
            values = {'chirality': get_chirality()}
            template = "{chirality}"
        print template.format(**values)

    elif args['set']:
        if args['internal']:
            ha = float(args['<HA>'])
            de = float(args['<DE>'])
            set_internal({'ha': ha, 'de': de}, args['blind'])
        elif args['celest']:
            ra = float(args['<RA>'])
            de = float(args['<DE>'])
            set_internal(celest_to_internal({'ra': ra, 'de': de}), args['blind'])
        elif args['star']:
            star = ephem.star(args['<NAME>'])
            star.compute(tican())
            set_internal(celest_to_internal({'ra': star.ra / math.pi * 12, 'de': star.dec / math.pi * 180}), args['blind'])
        elif args['cat']:
            celest = dagor_catalog.get_celest(args['<NAME>'], args['<CATALOG>'])
            set_internal(celest_to_internal(celest), args['blind'])

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


