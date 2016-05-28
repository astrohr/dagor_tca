#!/usr/bin/env python
# -*- coding: UTF-8 -*-

"""
Angle formats
=============

degrees
-------
12:34:45.6

hours
-----
14h25m36.7s

"""

from __future__ import division
import angles
import re


def parse_hours(value):
    h = 0
    m = 0
    s = 0
    value = value.strip()
    sign = -1 if value[0] == '-' else 1
    form1_1 = re.compile(r'^-?(([0-9]{1,2})(\.[0-9]*)?)h?$')
    form1_2 = re.compile(r'^-?([0-9]{1,2})h((([0-9]{1,2})(\.[0-9]*)?)m?)$')
    form1_3 = re.compile(r'^-?([0-9]{1,2})h([0-9]{1,2})m((([0-9]{1,2})(\.[0-9]*)?)s?)$')
    match = form1_1.match(value)
    if match:
        h = float(match.group(1))
    else:
        match = form1_2.match(value)
        if match:
            h = float(match.group(1))
            m = float(match.group(3))
        else:
            match = form1_3.match(value)
            if match:
                h = float(match.group(1))
                m = float(match.group(2))
                s = float(match.group(4))
    if not match:
        raise ValueError(u'Invalid hour format: {}'.format(value))
    if s >= 60:
        raise ValueError(u'Seconds must be less then 60')
    if m >= 60:
        raise ValueError(u'Minutes must be less then 60')
    return sign * (h + m / 60 + s / 3600)


def parse_degrees(value):
    d = 0
    m = 0
    s = 0
    value = value.strip()
    sign = -1 if value[0] == '-' else 1
    form1_1 = re.compile(ur'^-?(([0-9]{1,3})(\.[0-9]*)?):?$')
    form1_2 = re.compile(ur'^-?([0-9]{1,3}):((([0-9]{1,2})(\.[0-9]*)?):?)$')
    form1_3 = re.compile(ur'^-?([0-9]{1,3}):([0-9]{1,2}):(([0-9]{1,2})(\.[0-9]*)?)$')
    match = form1_1.match(value)
    if match:
        d = float(match.group(1))
    else:
        match = form1_2.match(value)
        if match:
            d = float(match.group(1))
            m = float(match.group(3))
        else:
            match = form1_3.match(value)
            if match:
                d = float(match.group(1))
                m = float(match.group(2))
                s = float(match.group(4))
    if not match:
        raise ValueError(u'Invalid degrees format: {}'.format(value))
    if s >= 60:
        raise ValueError(u'Seconds must be less then 60')
    if m >= 60:
        raise ValueError(u'Minutes must be less then 60')
    return sign * (d + m / 60 + s / 3600)


def format_hours(value):
    string_value = angles.fmt_angle(value, 'h', 'm', 's')
    if string_value[0] == '+':
        string_value = string_value[1:]
    return string_value


def format_degrees(value):
    string_value = angles.fmt_angle(value, ':', ':', '')
    if string_value[0] == '+':
        string_value = string_value[1:]
    return string_value

