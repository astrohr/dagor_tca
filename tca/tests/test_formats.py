#!/usr/bin/env python

from __future__ import division

import unittest

from formats import parse_hours


class TestFormats(unittest.TestCase):

    def test_parse_hours_int(self):
        h = parse_hours('14')
        self.assertEqual(h, 14)

    def test_parse_hours_h(self):
        h = parse_hours('14h')
        self.assertEqual(h, 14)

    def test_parse_hours_h_plus(self):
        h = parse_hours('+14h')
        self.assertEqual(h, 14)

    def test_parse_hours_h_minus(self):
        h = parse_hours('-14h')
        self.assertEqual(h, -14)

    def test_parse_hours_h_float(self):
        h = parse_hours('14.123h')
        self.assertEqual(h, 14.123)

    def test_parse_hours_h_too_much(self):
        with self.assertRaises(ValueError) as assertation:
            parse_hours('145')
        self.assertEqual('Invalid hour format: 145',
                         assertation.exception.message)

    def test_parse_hours_h_m(self):
        h = parse_hours('14h57')
        self.assertEqual(h, 14 + 57/60)

    def test_parse_hours_h_m_minutes(self):
        h = parse_hours('14h57m')
        self.assertEqual(h, 14 + 57/60)

    def test_parse_hours_h_m_1digit(self):
        h = parse_hours('14h5m')
        self.assertEqual(h, 14 + 5/60)

    def test_parse_hours_h_m_2digit(self):
        h = parse_hours('14h05m')
        self.assertEqual(h, 14 + 5/60)

    def test_parse_hours_h_m_over60(self):
        with self.assertRaises(ValueError) as assertation:
            parse_hours('14h65m')
        self.assertEqual('Minutes must be less then 60',
                         assertation.exception.message)

    def test_parse_hours_h_m_3digit(self):
        with self.assertRaises(ValueError) as assertation:
            parse_hours('14h012m')
        self.assertEqual('Invalid hour format: 14h012m',
                         assertation.exception.message)

    def test_parse_hours_h_m_negative(self):
        h = parse_hours('-14h05m')
        self.assertEqual(h, -14 - 5/60)



if __name__ == '__main__':
    unittest.main()