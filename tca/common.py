from __future__ import print_function, division, absolute_import

from datetime import datetime, timedelta
import sys
from time import sleep
import select
import math
import termios
import tty


BASE_PATH = "/home/dagor/tca/"

RESPONSES = {
    'OK': 0,
    'ERROR': 1,
    'BAD': 21,
    'IMPOSSIBLE_START': 31,
    'IMPOSSIBLE_END': 32,
    'IMPOSSIBLE_PATH_FAIL': 33,
    'NO_DIRECT': 34,
    'ENTER_ABORT': 900,
}


def exit_(status):
    print_(status)
    exit(RESPONSES[status])


verbose = True


def print_(msg, **kwargs):
    print(msg, **kwargs)
    sys.stdout.flush()


def p_(vars, end="\n"):
    sys.stdout.write("\n".join(["{k} = {v}".format(k=k, v=v) for k, v in vars.items()]))
    sys.stdout.write(end)
    sys.stdout.flush()


class EnterAbort(Exception):
    pass


def _heardEnter():
    i,o,e = select.select([sys.stdin],[],[], 0.0001)
    for s in i:
        if s == sys.stdin:
            input = sys.stdin.readline()
            return True
    return False


def wait_for_stop(motor, dots=False, after=0.1, interval=0.1, skip_dots=0, enter_abort=False, end="\n"):
    i = 0
    while True:
        i += 1
        sleep(interval)
        if _heardEnter():
            raise EnterAbort
        if dots:
            if not skip_dots or i % skip_dots == 0:
                sys.stdout.write('.')
                sys.stdout.flush()
        if motor.is_stopped:
            sleep(after)
            break
    sys.stdout.write(end)
    sys.stdout.flush()


def _wait_for_speed(motor, min=0, max=3000, dots=False, after=0.1, interval=0.1, skip_dots=0, enter_abort=False, end="\n"):
    i = 0
    while True:
        i += 1
        sleep(interval)
        if _heardEnter():
            raise EnterAbort
        if dots:
            if not skip_dots or i % skip_dots == 0:
                sys.stdout.write('.')
                sys.stdout.flush()
        if min <= motor.Speed <= max:
            sleep(after)
            break
    sys.stdout.write(end)
    sys.stdout.flush()


def wait_for_time(seconds=1, dots=False, interval=0.1, dot_skip=10, enter_abort=False, end="\n"):
    i = 0
    start_time = datetime.now()
    end_time = start_time + timedelta(seconds=seconds)
    while datetime.now() < end_time:
        i += 1
        sleep(interval)
        if enter_abort and _heardEnter():
            raise EnterAbort
        if dots and i >= dot_skip:
            i = 0
            sys.stdout.write('.')
            sys.stdout.flush()
    sys.stdout.write(end)
    sys.stdout.flush()


sign = lambda x: math.copysign(1, x)


class TimeoutError(Exception):
    pass


def timeout_call(func, args=(), kwargs=None, timeout_duration=1.0, default=None):
    if not kwargs:
        kwargs = {}
    import signal

    def handler(signum, frame):
        raise TimeoutError()

    # set the timeout handler
    signal.signal(signal.SIGALRM, handler)
    signal.setitimer(signal.ITIMER_REAL, timeout_duration)
    try:
        result = func(*args, **kwargs)
    except TimeoutError as exc:
        result = default
    finally:
        signal.alarm(0)

    return result


class NonBlockingConsole(object):

    def __enter__(self):
        self.old_settings = termios.tcgetattr(sys.stdin)
        tty.setcbreak(sys.stdin.fileno())
        return self

    def __exit__(self, type, value, traceback):
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.old_settings)

    def get_data(self):
        k = ''
        c = None
        while True:
            try:
                c = timeout_call(sys.stdin.read, args=[1], timeout_duration=0.05)
            except TimeoutError:
                pass
            if c is None:
                break
            k += c

        return k if k else False

