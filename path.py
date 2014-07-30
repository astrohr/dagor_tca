#!/usr/bin/env python
"""Dagor path and space calculation 0.2

Usage:
  path.py get
  path.py (path | move) from [--ring] <ha0> <de0> to <ha1> <de1> [--space=<file>] [--quiet]
  path.py test position [--ring] <ha> <de> [--space=<file>] [--quiet]
  path.py track [stop]
  path.py [-h | --help | help]
  path.py --version

Commands:
  test position     Check whether a position is allowed.
                    Exit with 0 if OK, exit with 21 if BAD.
  path from - to    Find the shortest allowed path between two points and echo to stdout
                    Output format:
                        ha1 de1
                        ha2 de2
                        ...
                        haN deN
                    Exit with 0 if path exists, exit with 31 if no allowed path exists.
  move from - to    Execute the path.
  track             Start tracking on current position.


Options:
  -h --help         Show this screen.
  --quiet           Only data on stdout
  --space=<file>    File defining allowed space region [default: space.txt]


"""

from __future__ import division
import re
from docopt import docopt
from shapely.geometry import Point, Polygon, LineString
import sys
from math import sqrt, floor
from astar import astar
from common import exit_

#   docopt    http://docopt.org/
#   shapely   http://toblerity.org/shapely/manual.html
#   astar     https://github.com/elemel/python-astar



_DEFAULT_SPACE_FILE = "/home/dagor/tca/space.txt"  # this should match docopt default for --space=<file>


class Position(object):
    ha = None
    de = None

    def __init__(self, ha=None, de=None, ring=False):
        if ring:
            self.parse_ring(ha, de)
        else:
            if ha is not None:
                self.ha = float(ha)
            if de is not None:
                self.de = float(de)

    def __repr__(self):
        return "({},{})".format(self.ha, self.de)

    def parse_ring(self, ha=None, de=None):
        if ha is not None:
            self.ha = float(ha)
            if self.ha > 24 and self.ha < 9:
                self.ha += 24
        if de is not None:
            val, sector = self.parse_ring_de(de)
            if sector == 1:
                self.de = val
            elif sector == 2:
                self.de = 180 - val
            elif sector == 3:
                self.de = 180 + val
            elif sector == 4:
                self.de = 360 - val
            else:
                raise ValueError("Invalid sector")

    def parse_ring_de(self, de):
        r = re.compile('^([0-9]{1,2})([\-\s_])([1-4]|((IV)|(III)|(II)|(I)))$')
        m = r.match(de)
        if m is None:
            raise ValueError("Can't Parse ring DE")
        val, sector = float(m.group(1)), m.group(3)
        if sector == 'I': 
            sector = 1
        elif sector == 'II':
            sector = 2
        elif sector == 'III':
            sector = 3
        elif sector == 'IV':
            sector = 4
        else:
            sector = int(sector)
        return val, sector

    def render_de_ring(self, de):
        raise NotImplementedError()

    def point(self):
        return Point(self.ha, self.de)


class Space(object):

    polygon = None

    def __init__(self):
        pass
            
    def load_file(self, fname):
        with open(fname) as f:
            lines = f.readlines()
        positions = []
        for line in lines:
            args = line.split()
            positions.append(Position(*args))
        self.polygon = Polygon([(p.ha, p.de) for p in positions])

    def contains(self, other):
        return self.polygon.contains(other)

    def path(self, p_0, p_1):

        HA_STEP = 0.5
        DE_STEP = 10
        bag = {}

        def cost(p_a, p_b):
            return sqrt((p_b.ha - p_a.ha) ** 2 + (p_b.de - p_a.de) ** 2)

        def heuristic(p):
            return cost(p, p_1)

        def neighbor_positions(p):
            local_grid = [
                (p.ha + HA_STEP, p.de),
                (p.ha, p.de + DE_STEP),
                (p.ha - HA_STEP, p.de),
                (p.ha, p.de - DE_STEP),
            ]
            for coords in local_grid:
                bag[coords] = bag.get(coords, Position(coords[0], coords[1]))

            return [bag[coords] for coords in local_grid if self.contains(bag[coords].point())]

        def goal(p):
            near_area = Polygon([
                (p.ha - HA_STEP/2, p.de - DE_STEP/2),
                (p.ha - HA_STEP/2, p.de + DE_STEP/2),
                (p.ha + HA_STEP/2, p.de + DE_STEP/2),
                (p.ha + HA_STEP/2, p.de - DE_STEP/2),
            ])
            return near_area.contains(p_1.point()) or near_area.touches(p_1.point())  # touches if point exactly on the grid

        def smooth(path):

            n = len(path) - 1
            i = 0
            direct_path = [0]

            def find_furthest_direct():
                j = n
                k = n
                best_j = i + 1
                while True:
                    line = LineString([(path[i].ha, path[i].de), (path[j].ha, path[j].de)])
                    if self.contains(line):
                        best_j = max(j, best_j)
                        j += int(floor((k-j)/2))
                        if k - best_j < 2:
                            return best_j
                    else:
                        k = j
                        j -= int(floor((j-best_j)/2))
                        if k - j < 1:
                            return best_j

            while n - i > 2:
                j = find_furthest_direct()
                direct_path.append(j)
                i = j

            return [path[i] for i in range(0, len(path)) if i in direct_path]

        path = astar(p_0, neighbor_positions, goal, 0, cost, heuristic)
        if path == []:
            path = [p_0, p_1]
        else:
            path[-1] = p_1
        return smooth(path)


def _get_space(space_filepath=None):
    if space_filepath is None:
        space_filepath = _DEFAULT_SPACE_FILE
    space = Space()
    space.load_file(space_filepath)
    return space


# interface

class NoPath(Exception):
    pass

class ImpossibleStart(NoPath):
    pass

class ImpossibleEnd(NoPath):
    pass


def get_path(start_internal, end_internal, space=None):
    """
    Use internal coordinates only!
    """
    if space is None:
        space = _get_space()
    p_0 = Position(start_internal['ha'], start_internal['de'])
    p_1 = Position(end_internal['ha'], end_internal['de'])
    line = LineString([(p_0.ha, p_0.de), (p_1.ha, p_1.de)])
    line_is_in = space.polygon.contains(line)
    if not space.contains(p_0.point()):
        raise ImpossibleStart()
    if not space.contains(p_1.point()):
        raise ImpossibleEnd()
    if not line_is_in:
        path = space.path(p_0, p_1)
        if path is None:
            raise NoPath()
    else:
        path = [p_0, p_1]
    return path

# Run as CLI client

def _main(args):

    if args['test'] and args['position']:
        space = _get_space(args['--space'])
        position = Position(args['<ha>'], args['<de>'], args['--ring'])
        if space.contains(position.point()):
            exit_("OK")
        else:
            exit_("BAD")

    if (args['path'] or args['move']) and args['from'] and args['to']:
        ha_0 = args['<ha0>']
        de_0 = args['<de0>']
        ha_1 = args['<ha1>']
        de_1 = args['<de1>']
        try:
            space = _get_space(args['--space'])
            path = get_path({'ha': ha_0, 'de': de_0}, {'ha': ha_1, 'de': de_1}, space)
        except ImpossibleStart:
            exit_('IMPOSSIBLE_START')
        except ImpossibleEnd:
            exit_('IMPOSSIBLE_END')
        except NoPath:
            exit_('IMPOSSIBLE_PATH_FAIL')
        else:
            for position in path:
                print "%f %f" % (position.ha, position.de)
            if len(path) == 2:
                exit_('OK')
            else:
                exit_('NO_DIRECT')







_verbose = True


def _print(msg):
    if _verbose:
        print(msg)


if __name__ == '__main__':

    args = docopt(__doc__, version=__doc__.split("\n")[0])

    if len(sys.argv) == 1 or args['help']:
        print __doc__.strip()
        exit(0)

    if args['--quiet']:
        _verbose = False

    _main(args)


