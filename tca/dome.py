#!/usr/bin/env python
"""Dagor dome rotation interface version 1.3.

Usage:
    dome.py server
    dome.py status [-v | --verbose]
    dome.py calibrate
    dome.py goto <azimuth>
    dome.py azimuth set <azimuth>
    dome.py azimuth get
    dome.py park
    dome.py door (open|close|stop)
    dome.py home (set|get)
    dome.py (up|down|stop)
    dome.py -h | --help
    dome.py --version

Commands:
    server         Start Flusk server.
    status         Dome status (azimuth \\n rotating or not \\n calibrated, during calibration or uncalibrated).
    calibrate      Start calibration.
    goto           Move dome to specified azimuth.
    azimuth set    Set specified azimuth as current position.
    azimuth get    Get current azimuth.
    park           Move dome to home azimuth.
    door           Open or close door.
    home set       Set home to current azimuth.
    home get       Get home azimuth.
    up             Start rotation of the dome (south-east-north-west).
    down           Start rotation of the dome (south-wast-north-east).
    stop           Stop rotation of the dome.

Options:
    -h --help      Show this screen.
    --version      Show version.
"""
  
from docopt import docopt
import serial
import sys
from os import system
import local.dome_config
from flask import Flask
from flask import request

app = Flask(__name__)


PORT = local.dome_config.PORT
BAUDRATE = 9600
TIMEOUT = 2


def read_serial(ser):
    """ Read data from serial port. Raise exception if error code is returned.
    
    @param ser: PySerial Serial() object
    @return: Returned data
    """
    
    raw_reply = ser.readline().strip()
    
    try:
        status, lines = raw_reply.split(' ')
    except ValueError:
        status, lines = (raw_reply, 0)
    
    reply = [ser.readline().strip() for _ in range(int(lines))]
    
    if status=='ok':
        return reply
    else:
        raise Exception(reply)
    

def write_serial(ser, str):
    """ Write to serial port, and then parse return message.
    
    @param ser: PySerial Serial() object.
    @param str: String to be sent.
    @return: Parsed return message.
    """
    
    ser.write(str + '\n')
    return read_serial(ser)

class operation:
    def __init__(self):
        self.status = False
        self.status_verbose = False
        self.calibrate = False
        self.goto = False
        self.goto_azimuth = 0
        self.azimuth_set = False
        self.azimuth_set_azimuth = 0
        self.azimuth_get = False
        self.park = False
        self.door_open = False
        self.door_close = False
        self.door_stop = False
        self.home_set = False
        self.home_get = False
        self.up = False
        self.down = False
        self.stop = False

def execute_serial(operation):
    """ Execute specified operation over serial and return info as string.

    @param operation: operation to be executed.
    @return String with any error or info messages parsed from serial port.
    """

    ser = serial.Serial()
    ser.port = PORT
    ser.baudrate = BAUDRATE
    ser.timeout = TIMEOUT

    ser.open()
    
    return_str = ""
    
    if operation.status:
        reply = write_serial(ser, 'status')
        if operation.status_verbose:
            position, rotation, calibration = reply
            return_str =  'Azimuth: ' + str(position) + "\n" + 'Rotation: ' + {'0': 'Stop', '1': 'Down', '-1': 'Up'}[rotation] + "\n" + 'Calibration: ' + {'0': 'In progress', '1': 'Done', '-1': 'Not calibrated'}[calibration]
        else:
            return_str =  '|'.join(reply)
    elif operation.calibrate:
        write_serial(ser, 'calibrate')
    elif operation.goto:
        write_serial(ser, 'goto' + operation.goto_azimuth)
    elif operation.azimuth_set:
        write_serial(ser, 'force' + operation.azimuth_set_azimuth)
    elif operation.azimuth_get:
        return_str =  '\n'.join(write_serial(ser, 'current_azimuth'))
    elif operation.park:
        write_serial(ser, 'park')
    elif operation.door_open:
        write_serial(ser, 'door_open')
    elif operation.door_stop:
        write_serial(ser, 'door_stop')
    elif operation.door_close:
        write_serial(ser, 'door_close')
    elif operation.home_set:
        write_serial(ser, 'set_as_home')
    elif operation.home_get:
        return_str =  '\n'.join(write_serial(ser, 'home_azimuth'))
    elif operation.up:
        write_serial(ser, 'up')
    elif operation.down:
        write_serial(ser, 'down')
    elif operation.stop:
        write_serial(ser, 'stop')
    
    ser.close()
    
    return return_str

def _parse_args(args):
    """ Parse command line arguments, and call execute_serial()

    @param args: docopt array of command line arguments
    @return: String returned from excute_serial()
    """

    op = operation()

    if args['status']:
        op.status = True
        if args['-v'] or args['--verbose']:
            op.status_verbose = True
    elif args['calibrate']:
        op.calibrate = True
    elif args['goto']:
        op.goto = True
        op.goto_azimuth = args['<azimuth>']
    elif args['azimuth']:
        if args['set']:
            op.azimuth_set = True
            op.azimuth_set_azimuth = args['<azimuth>']
        elif args['get']:
            op.azimuth_get = False
        else:
            _unknown_command()
    elif args['park']:
        op.park = True
    elif args['door']:
        if args['open']:
            op.door_open = True
        elif args['stop']:
            op.door_stop = True
        elif args['close']:
            op.door_close = True
        else:
            _unknown_command()
    elif args['home']:
        if args['set']:
            op.home_set = True
        elif args['get']:
            op.home_get = True
        else:
            _unknown_command()
    elif args['up']:
        op.up = True
    elif args['down']:
        op.down = True
    elif args['stop']:
        op.stop = True
    else:
        _unknown_command()

    return execute_serial(op)
    
def _unknown_command():
    print __doc__
    sys.exit(1)
    
@app.route('/dome', methods=['GET', 'POST'])
def _parse_http():
    """ Parse HTTP request (GET or POST), and call execute_serial()

    @return: String returned from excute_serial()
    """

    op = operation()

    if request.method == 'GET':
        if 'status' in request.args:
            op.status = True
            if request.args['status'] == 'verbose':
                op.status_verbose = True
        elif 'azimuth' in request.args:
            op.azimuth_get = True
        elif 'home' in request.args:
            op.home_get = True
    elif request.method == 'POST':
        if 'calibrate' in request.json:
            op.calibrate = True
        elif 'goto' in request.json:
            op.goto = True
            op.goto_azimuth = request.json['goto']
        elif 'azimuth' in request.json:
            op.azimuth_set = True
            op.azimuth_set_azimuth = request.json['azimuth']
        elif 'park' in request.json:
            op.park = True
        elif 'door' in request.json:
            if request.json['door'] == 'open':
                op.door_open = True
            elif request.json['door'] == 'stop':
                op.door_stop = True
            elif request.json['door'] == 'close':
                op.door_close = True
        elif 'home' in request.json:
            op.home_set = True
        elif 'up' in request.json:
            op.up = True
        elif 'down' in request.json:
            op.down = True
        elif 'stop' in request.json:
            op.stop = True

    str = execute_serial(op)
    return str


if __name__ == '__main__':
    args = docopt(__doc__, version=__doc__.split('\n'[0]))
    
    if(args['server']):
        app.run(host='0.0.0.0', port=4242)
    else:
        try:
            print _parse_args(args)
            sys.exit(0)
        except Exception as e:
            print(e)
            sys.exit(1)
