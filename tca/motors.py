#!/usr/bin/env python
"""Dagor motors control 0.2

Usage:
  motors.py [ha | de] manual
  motors.py [ha | de] alarm
  motors.py [ha | de] status
  motors.py [ha | de] enabled
  motors.py [ha | de] speed
  motors.py (ha | de) speed set <N>
  motors.py [ha | de] stop
  motors.py [ha | de] enable
  motors.py [ha | de] disable
  motors.py [ha | de] reset
  motors.py (ha | de) move by <angle> [pulse] [speed <S>]
  motors.py [ha | de] configure
  motors.py [-h | --help | help]
  motors.py --version

Commands:
  status            Get motor statuses.
  speed             Get speed (rpm)
  speed set <N>     Set speed (rpm)
  enable            Enable motor
  disable           Disable motor
  configure         Write default configuration to motor drives.

Options:
  -h --help         Show this screen.
  --quiet           Only data on stdout


"""
from __future__ import division
from docopt import docopt

import serial

import modbus_tk.defines as cst
from modbus_tk import modbus
from modbus_tk import modbus_rtu
from time import sleep
import sys

from position import HA_SLOPE, DE_SLOPE
import local.configuration as config


_DE_MOTOR_TO_ENCODER_RATIO = 97.6185
_HA_MOTOR_TO_ENCODER_RATIO = 303.355

_TRACKING_ERROR_FEEDBACK_ha = 0.5
_TRACKING_ERROR_FEEDBACK_de = 150

MAXI = 32767
MINI = -32768
FULL = 65536
FULL2 = FULL**2
FULL2_MAXI = FULL2 / 2 - 1
FULL2_MINI = FULL2 / -2
MAX_TASKS = 2

SPEED_LIMIT = config.MOTORS['speed_limit']

MAX_SPEED_DE = 1.456  # deg/sec
MAX_SPEED_HA = 3.075e-2  # h/sec
ACC_RAMP = 7000
EMERGENCY_RAMP = 3000  # milliseconds at full speed
# ^^^ !!! also adjust power-off relay timer !!! ^^^
TRACKING_SPEED = -27

OP_MODE_MANUAL = 0
OP_MODE_SPEED = 1
OP_MODE_POSITION = 4


class ModbusProtocol(object):
    # Serial port configuration
    _PORT = "/dev/ttyS1"
    _BAUD_RATE = 19200
    _BYTE_SIZE = serial.EIGHTBITS
    _PARITY = serial.PARITY_EVEN
    _STOP_BITS = serial.STOPBITS_ONE
    _TIMEOUT = 0.2

    def __init__(self):
        super(ModbusProtocol, self).__init__()
        self.master = modbus_rtu.RtuMaster(
            serial.Serial(
                ModbusProtocol._PORT,
                ModbusProtocol._BAUD_RATE,
                ModbusProtocol._BYTE_SIZE,
                ModbusProtocol._PARITY,
                ModbusProtocol._STOP_BITS,
                ModbusProtocol._TIMEOUT,
            ))

    def write_registers(self, motor_address, start_register, data):
        e = None
        for i in range(1, 5):
            try:
                self.master.execute(
                    motor_address,
                    cst.WRITE_MULTIPLE_REGISTERS,
                    start_register,
                    output_value=data,
                )
            except modbus.ModbusError, e:
                pass
            except modbus.ModbusInvalidResponseError, e:
                pass
            else:
                return
        if e:
            if isinstance(e, modbus.ModbusError):
                print("%s- Code=%d" % (e, e.get_exception_code()))
            elif isinstance(e, modbus.ModbusInvalidResponseError):
                print("InvalidResponseError while writing to "
                      "register {} of motor {}"
                      .format(start_register, motor_address))
            print(e)
            raise e

    def read_registers(
            self, motor_address, start_register, number_of_registers):
        e = None
        for i in range(1, 5):
            try:
                return self.master.execute(
                    motor_address,
                    cst.READ_HOLDING_REGISTERS,
                    start_register,
                    number_of_registers,
                    data_format=">" + (number_of_registers * "h"),
                )
            except modbus.ModbusError, e:
                print("%s- Code=%d" % (e, e.get_exception_code()))
                pass
            except modbus.ModbusInvalidResponseError, e:
                print("InvalidResponseError while reading "
                      "register {} of motor {}"
                      .format(start_register, motor_address))
                print(e)
                pass
        if e:
            raise e


class OverflowException(ValueError):
    pass


class MotorParameter(object):
    def __init__(self, register_number, size):
        self.register_number = register_number
        self.size = size

    def process_set(self, value, mc):
        # noop
        return int(value)

    def process_get(self, value, mc):
        # noop
        return value


class SetSpeedMotorParameter(MotorParameter):
    def process_set(self, value, mc):
        limit = mc.SpeedLimit
        value = max(min(value, limit), -limit)
        return int(value / limit * 32767)

    def process_get(self, value, mc):
        limit = mc.SpeedLimit
        return int(value * limit / 32767)


class ReadonlyMotorParameter(MotorParameter):
    def process_set(self, value, mc):
        raise AttributeError('Read-only parameter')


class MotorCommunication(object):
    DGTINSET = MotorParameter(73, 1)
    DGTINCLR = MotorParameter(74, 1)
    DGTSTAT = MotorParameter(65, 1)
    DGTIN2Conf = MotorParameter(104, 1)
    DGTIN2 = MotorParameter(128, 1)
    DGTIN3Conf = MotorParameter(87, 1)
    DGTIN3 = MotorParameter(129, 1)
    DGTIN4Conf = MotorParameter(88, 1)
    DGTIN4 = MotorParameter(130, 1)
    DGTIN5Conf = MotorParameter(105, 1)
    DGTIN5 = MotorParameter(131, 1)
    DGTOUT1Conf = MotorParameter(125, 1)
    DGTOUT1 = MotorParameter(136, 1)
    DGTOUT2Conf = MotorParameter(126, 1)
    DGTOUT2 = MotorParameter(137, 1)
    ALARM = MotorParameter(51, 2)
    HALARM = MotorParameter(83, 2)

    SpeedLimit = MotorParameter(31, 1)
    SpeedAccRamp = MotorParameter(34, 1)
    SpeedDecRamp = MotorParameter(35, 1)
    SpeedEmerRamp = MotorParameter(36, 1)

    OperationMode = MotorParameter(71, 1)
    Position = ReadonlyMotorParameter(134, 2)
    Speed = ReadonlyMotorParameter(59, 1)
    SetSpeed = SetSpeedMotorParameter(80, 1)
    SetHomingType = MotorParameter(91, 1)

    AnalogFilter = MotorParameter(17, 1)  # ms
    AnalogDeadBand = MotorParameter(28, 1)  # mV

    COMMAND_READ_EEPROM = 1  # configuration
    COMMAND_WRITE_EEPROM = 2
    COMMAND_WRITE_FLASH = 32  # task configuration
    COMMAND_READ_FLASH = 64
    Commands = MotorParameter(69, 1)

    tasks = range(1, MAX_TASKS+1)

    ALARM_NAMES = [
        # LO:
        'Overcurrent regen resistance alarm',  # 1
        'Holding brake alarm',  # 2
        'In - rush bus alarm',  # 3
        'Auxiliary voltage alarm',  # 4
        'NA',  # 5
        'NA',  # 6
        'Flash alarm',  # 7
        'CanBus alarm'  # 8
        'NA',  # 9
        'Homing alarm',  # 10
        'NA',  # 11
        'NA',  # 12
        'NA',  # 13
        'NA',  # 14
        'NA',  # 15
        # HI:
        'Eeprom alarm',  # 0
        'Overcurrent alarm',  # 1
        'Drive temperature alarm',  # 2
        'Hall alarm',  # 3
        'Encoder alarm',  # 4
        'I2t drive alarm',  # 5
        'Motor temperature alarm',  # 6
        'Regen resistance alarm',  # 7
        'Min / Max voltage alarm',  # 8
        'NA',  # 9
        'NA',  # 10
        'Resolver alarm',  # 11
        'NA',  # 12
        'Following error alarm',  # 13
        'Limit switch alarm',  # 14
        'NA',  # 15
    ]

    def __init__(self, protocol, address):
        self.protocol = protocol
        self.address = address
        set_attr_on_self = super(MotorCommunication, self).__setattr__
        for i in range(1, MAX_TASKS + 1):
            set_attr_on_self('Task{}_Target'.format(i),
                             MotorParameter(320 + 16 * (i-1), 2))
            set_attr_on_self('Task{}_Speed'.format(i),
                             MotorParameter(322 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_Acc_Time'.format(i),
                             MotorParameter(323 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_Dcc_Time'.format(i),
                             MotorParameter(324 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_Window_Pos'.format(i),
                             MotorParameter(325 + 16 * (i-1), 2))
            set_attr_on_self('Task{}_Window_Time'.format(i),
                             MotorParameter(327 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_Next_Profile'.format(i),
                             MotorParameter(328 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_Delay_Time'.format(i),
                             MotorParameter(329 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_Settings'.format(i),
                             MotorParameter(334 + 16 * (i-1), 1))
            set_attr_on_self('Task{}_State'.format(i),
                             MotorParameter(335 + 16 * (i-1), 1))

    def __getattribute__(self, key):
        """poor man's descriptor pattern, getter"""
        item = super(MotorCommunication, self).__getattribute__(key)
        if isinstance(item, MotorParameter):
            return self._read(item)
        else:
            return item

    def __setattr__(self, key, value):
        """poor man's descriptor pattern, setter"""
        try:
            item = super(MotorCommunication, self).__getattribute__(key)
        except AttributeError:
            item = None
        if isinstance(item, MotorParameter):
            self._write(item, value)
        else:
            super(MotorCommunication, self).__setattr__(key, value)

    def _param(self, key):
        return super(MotorCommunication, self).__getattribute__(key)

    def configure(self):
        self.configure_dgtio()
        self.configure_speed()
        self.configure_tasks()
        self.OperationMode = OP_MODE_MANUAL
        self.Commands = self.COMMAND_WRITE_EEPROM

    def configure_flash(self):
        """
        Take care: writing to flash memory interrupts modbus communication
        """
        self.Commands = self.COMMAND_WRITE_FLASH

    def configure_dgtio(self):
        # self.DGTOUT1Conf = 3  # Homing ok
        self.DGTOUT1Conf = 2  # Speed < ...
        self.DGTOUT1 = 1  # Speed <= this
        self.DGTOUT2Conf = 7  # Target ok
        self.SetHomingType = 35  # immediate homing
        # DGTIN1 is Enable
        self.DGTIN2Conf = 13  # start homing
        self.DGTIN3Conf = 8  # task 1
        self.DGTIN3 = 1
        self.DGTIN4 = 0
        self.DGTIN4Conf = 14  # Reset Fault
        self.DGTIN5Conf = 12  # Emergency

    def configure_speed(self):
        self.SpeedLimit = SPEED_LIMIT  # Speed limit 3000 rpm
        self.SpeedAccRamp = ACC_RAMP  # Speed mode acceleration ramp 5 s
        self.SpeedDecRamp = ACC_RAMP  # Speed mode deceleration ramp 5 s
        self.SpeedEmerRamp = EMERGENCY_RAMP  # Speed emergency ramp 500 ms
        self.AnalogFilter = 2000
        self.AnalogDeadBand = 3500

    def configure_tasks(self):
        for i in range(1, MAX_TASKS+1):
            self._write(self._param('Task{}_Speed'.format(i)), SPEED_LIMIT/10)
            self._write(self._param('Task{}_Acc_Time'.format(i)), ACC_RAMP)
            self._write(self._param('Task{}_Dcc_Time'.format(i)), ACC_RAMP)
            self._write(self._param('Task{}_Target'.format(i)), 0)
            self._write(self._param('Task{}_Window_Pos'.format(i)), 50)
            self._write(self._param('Task{}_Window_Time'.format(i)), 500)
            self._write(self._param('Task{}_Next_Profile'.format(i)), 0)
            self._write(self._param('Task{}_Delay_Time'.format(i)), 0)
        self.OperationMode = OP_MODE_POSITION
        if self.is_enabled:
            self.disable()
            sleep(1)
            if self.is_enabled:
                raise Exception(
                    "Can't disable drive {}! Is is manually enabled?"
                    .format({1: 'HA', 2: 'DE'}[self.address]))
        self.enable()
        self.set_homing()
        sleep(1)
        self.clear_homing()
        self.clear_start_task()
        self.disable()

    def alarms_str(self):
        alarm = self.ALARM
        present_alarms = []
        for i, alarm_name in enumerate(self.ALARM_NAMES):
            # noinspection PyUnresolvedReferences
            if alarm & 2**i:
                present_alarms.append("AL{}: {}".format(i, alarm_name))
        return present_alarms

    def status_str(self):
        bool_en = lambda v: "Enabled" if v else "Disabled"
        template = "{en}, alarms: {al}"
        values = {
            'en': bool_en(self.is_enabled),
            'al': self.alarms_str(),
        }
        return template.format(**values)

    def enable(self):
        self.DGTINSET |= 1

    def disable(self):
        if not self.is_stopped:
            print("Refusing to disable moving drive!")
            return
        self.DGTINCLR |= 1

    def reset(self):
        if not self.is_stopped:
            print("Refusing to reset moving drive!")
            return
        self.disable()
        self.DGTIN4 = 1
        self.enable()

    @property
    def is_enabled(self):
        # noinspection PyTypeChecker
        return not self.DGTSTAT % 2 == 0

    def get_motor_position(self):
        return self.GetPosition

    def get_homing_ok(self):
        # noinspection PyTypeChecker
        return not (self.DGTSTAT / 16384) % 2 == 0

    def get_target_ok(self):
        # noinspection PyTypeChecker
        return not (self.DGTSTAT / 32768) % 2 == 0

    def set_homing(self):
        self.DGTINSET |= 2

    def clear_homing(self):
        self.DGTINCLR |= 2

    def set_start_task(self):
        self.DGTINSET |= 4

    def clear_start_task(self):
        self.DGTINCLR |= 4

    @property
    def is_start_task(self):
        # noinspection PyTypeChecker
        return self.DGTSTAT & 4 == 4

    @property
    def is_stopped(self):
        # noinspection PyUnresolvedReferences
        return self.DGTSTAT & 2**14 == 2**14

    def move_delta(self, delta_position, case=None):
        if not self.OperationMode == OP_MODE_POSITION:
            raise ValueError('Invalid OperationMode!')
        if not self.is_stopped:
            raise ValueError('Drive is moving!')
        if not self.is_enabled:
            self.enable()
        if case == 'ha':
            delta_position *= HA_SLOPE * _HA_MOTOR_TO_ENCODER_RATIO
        if case == 'de':
            delta_position *= DE_SLOPE * _DE_MOTOR_TO_ENCODER_RATIO
        self.clear_start_task()
        self.clear_homing()
        self.set_homing()
        target = int(delta_position)
        if target > FULL2_MAXI or target < FULL2_MINI:
            raise ValueError(
                'Target outside of range, but I mean way outside!')
        # noinspection PyAttributeOutsideInit
        self.Task1_Target = target
        self.set_start_task()

    def _read(self, parameter_register):
        data = self.protocol.read_registers(
            self.address,
            parameter_register.register_number,
            parameter_register.size,
        )[::-1]
        value = sum([
            (data[i] if data[i] >= 0 else data[i] + FULL) * FULL ** i
            for i in range(0, parameter_register.size)
        ])
        i = parameter_register.size
        if value > MAXI**i / i:
            value = value - FULL**i
        value = parameter_register.process_get(value, self)
        return value

    def _write(self, parameter_register, value):
        value = parameter_register.process_set(value, self)
        data = [0] * parameter_register.size
        for i in range(0, parameter_register.size):
            chunk = (value % FULL ** (i+1)) / FULL ** i
            data[i] = int(chunk if chunk < FULL/2 else chunk - FULL)
        self.protocol.write_registers(
            self.address, parameter_register.register_number, data[::-1])


# Control logic

_inited = False
_ha = None
_de = None


def init():
    global _inited, _ha, _de
    if _inited:
        return None
    _inited = True
    motors_comm_protocol = ModbusProtocol()
    _ha = MotorCommunication(motors_comm_protocol, 1)
    _de = MotorCommunication(motors_comm_protocol, 2)


def get_motor(motor):
    global _inited, _ha, _de
    if not _inited:
        raise RuntimeError('motors not yet inited')
    ha, de = _ha, _de

    if isinstance(motor, MotorCommunication):
        return motor
    if isinstance(motor, basestring) and motor in ('ha', 'de',):
        return locals()[motor]
    raise ValueError(
        'Argument `motor` must be either'
        ' MotorCommunication instance or "ha" or "de"')


def enable(motor):
    motor = get_motor(motor)
    motor.enable()


def disable(motor):
    motor = get_motor(motor)
    motor.disable()


def configure():
    _ha.disable()
    _de.disable()
    _ha.configure()
    _de.configure()
    # order is important when writing to flash (not eeprom):
    _ha.configure_flash()
    _de.configure_flash()


def require_stopped(motor):
    motor = get_motor(motor)
    if not stopped(motor):
        raise ValueError('Can not do that while drive is moving!')


def require_op_mode(motor, op_mode):
    motor = get_motor(motor)
    if not motor.OperationMode == op_mode:
        raise ValueError('Wrong operation mode!')


def set_speed(motor, speed):
    motor = get_motor(motor)
    if not stopped(motor):
        require_op_mode(motor, OP_MODE_SPEED)
    motor.OperationMode = OP_MODE_SPEED
    motor.SetSpeed = speed


def move_by(delta_local, speeds=None):
    global _inited, _ha, _de
    if speeds is None:
        speeds = {'ha': SPEED_LIMIT, 'de': SPEED_LIMIT}
    if delta_local.get('ha', False):
        require_stopped(_ha)
        _ha.enable()
        case = 'ha'
        delta_position = float(delta_local['ha'])
        _ha.OperationMode = OP_MODE_POSITION
        _ha.Task1_Speed = abs(speeds['ha'])
        _ha.move_delta(delta_position, case=case)
    if delta_local.get('de', False):
        require_stopped(_de)
        _de.enable()
        case = 'de'
        delta_position = float(delta_local['de'])
        _de.OperationMode = OP_MODE_POSITION
        _de.Task1_Speed = abs(speeds['de'])
        _de.move_delta(delta_position, case=case)


def stopped(motor):
    motor = get_motor(motor)
    return motor.is_stopped and motor.Speed == 0


def moving():
    global _inited, _ha, _de
    return not stopped(_ha) and not stopped(_de)


def stop(ha=None, de=None):
    global _inited, _ha, _de
    if ha is None and de is None:
        ha = True
        de = True
    if ha:
        if not stopped(_ha):
            if _ha.OperationMode == OP_MODE_SPEED:
                _ha.SetSpeed = 0
            elif _ha.OperationMode == OP_MODE_POSITION:
                _ha.clear_start_task()
            else:
                raise ValueError('Unsupported operation mode!')
    if de:
        if not stopped(_de):
            if _de.OperationMode == OP_MODE_SPEED:
                _de.SetSpeed = 0
            elif _de.OperationMode == OP_MODE_POSITION:
                _de.clear_start_task()
            else:
                raise ValueError('Unsupported operation mode!')


def set_manual(ha=None, de=None):
    if ha is None and de is None:
        ha = True
        de = True
    init()
    if ha:
        require_stopped(_ha)
        _ha.disable()
        _ha.OperationMode = OP_MODE_MANUAL
    if de:
        require_stopped(_de)
        _de.disable()
        _de.OperationMode = OP_MODE_MANUAL


def status_str(ha=None, de=None):
    global _inited, _ha, _de
    init()
    if ha is None and de is None:
        ha = True
        de = True
    if ha:
        print "ha: {}".format(_ha.status_str())
    if de:
        print "de: {}".format(_de.status_str())


def reset(ha=None, de=None):
    global _inited, _ha, _de
    init()
    if ha is None and de is None:
        ha = True
        de = True
    if ha:
        _ha.reset()
    if de:
        _de.reset()


# Run as CLI client


def _main(args):
    init()

    if args['configure']:
        if not args['ha'] and not args['de']:
            args['ha'] = True
            args['de'] = True
        if args['ha']:
            _ha.configure()
        if args['de']:
            _de.configure()

    if args['enable']:
        if not args['ha'] and not args['de']:
            args['ha'] = True
            args['de'] = True
        if args['ha']:
            _ha.enable()
        if args['de']:
            _de.enable()

    if args['enabled']:
        values = {}
        template = "ha={ha}\nde={de}"
        if not args['ha'] and not args['de']:
            values = {
                'ha': _ha.is_enabled,
                'de': _de.is_enabled,
            }
        if args['ha']:
            values = {
                'ha': _ha.is_enabled,
            }
            template = "{ha}"
        if args['de']:
            values = {
                'de': _de.is_enabled,
            }
            template = "{de}"
        print template.format(**values)

    if args['disable']:
        if not args['ha'] and not args['de']:
            args['ha'] = True
            args['de'] = True
        if args['ha']:
            _ha.disable()
        if args['de']:
            _de.disable()

    if args['reset']:
        if not args['ha'] and not args['de']:
            args['ha'] = True
            args['de'] = True
        if args['ha']:
            _ha.reset()
        if args['de']:
            _de.reset()

    if args['alarm']:
        if not args['ha'] and not args['de']:
            print _ha.ALARM
            print _de.ALARM
        if args['ha']:
            print _ha.ALARM
        if args['de']:
            print _de.ALARM

    if args['status']:
        status_str(args['ha'] or None, args['de'] or None)

    if args['manual']:
        if not args['ha'] and not args['de']:
            args['ha'] = True
            args['de'] = True
        set_manual(ha=args['ha'], de=args['de'])

    if args['speed']:
        if not args['set']:
            template = "ha={ha}\nde={de}"
            values = {}
            if not args['ha'] and not args['de']:
                values = {
                    'ha': _ha.Speed,
                    'de': _de.Speed,
                }
            if args['ha']:
                values['ha'] = _ha.Speed
                template = "{ha}"
            if args['de']:
                values['de'] = _de.Speed
                template = "{de}"
            print template.format(**values)
        else:
            speed = int(args['<N>'])
            motor = None
            if args['ha']:
                motor = _ha
            if args['de']:
                motor = _de
            set_speed(motor, speed)

    if args['stop']:
        if not args['ha'] and not args['de']:
            args['ha'] = True
            args['de'] = True
        if args['ha']:
            if not stopped(_ha):
                if _ha.OperationMode == OP_MODE_SPEED:
                    _ha.SetSpeed = 0
                elif _ha.OperationMode == OP_MODE_POSITION:
                    _ha.clear_start_task()
                else:
                    raise ValueError('Unsupported operation mode!')
        if args['de']:
            if not stopped(_de):
                if _de.OperationMode == OP_MODE_SPEED:
                    _de.SetSpeed = 0
                elif _de.OperationMode == OP_MODE_POSITION:
                    _de.clear_start_task()
                else:
                    raise ValueError('Unsupported operation mode!')

    if args['move'] and args['by']:
        if args['ha']:
            if not args['pulse']:
                case = 'ha'
            else:
                case = None
            # TODO parse angle formats:
            delta_position = -1 * float(args['<angle>'])
            _ha.OperationMode = OP_MODE_POSITION
            if args['<S>']:
                _ha.Task1_Speed = int(args['<S>'])
            _ha.move_delta(delta_position, case=case)
        if args['de']:
            require_stopped(_de)
            if not args['pulse']:
                case = 'de'
            else:
                case = None
            # TODO parse angle formats:
            delta_position = -1 * float(args['<angle>'])
            _de.OperationMode = OP_MODE_POSITION
            if args['<S>']:
                _de.Task1_Speed = int(args['<S>'])
            _de.move_delta(delta_position, case=case)


if __name__ == '__main__':

    cli_args = docopt(
        __doc__, version=__doc__.split("\n")[0], options_first=True)

    if len(sys.argv) == 1 or cli_args['help']:
        print __doc__.strip()
        exit(0)

    _main(cli_args)
