# coding=utf-8


STELLARIUM_MODE_DATE = 'on date'
STELLARIUM_MODE_J2000 = 'J2000.0'

MOTORS = {
    'speed_limit': 3000,
    'port': '/dev/ttyS0'
}

TRACKING = {
    #'total_speed_limit': 4250,
    'total_speed_limit': 6000,
    'enable_rnd': False,
    'stellarium_mode': STELLARIUM_MODE_J2000,
    'tracking_check_interval': 0.1,  # seconds
    'tracking_target_file': '/home/dagor/tca/local/var/tracking_config.json',
    'tracking_current_file': '/home/dagor/tca/local/var/tracking_current.json',
}


DOME_CONFIG = {
    'port': "/dev/dagor_dome",
    'baud_rate': 9600,
    'timeout': 1,  # if no data available, block for max this many seconds
    'retries': 5,
    'interval': 1,  # seconds
    'reset_disabled': True,
}


FOCUS_CONFIG = {
    'port': '/dev/dagor_focus',
    'baud_rate': 115200,
    'timeout': 0.1,  # if no data available, block for max this many seconds
    'retries': 5,
    'interval': 1,  # seconds
    'reset_disabled': True,
}


FANS_CONFIG = {
    'port': '/dev/dagor_fans',
    'reset_disabled': True,
}


LIGHTS_CONFIG = {
    'port': "/dev/dagor_lights",
    'reset_disabled': True,
}

