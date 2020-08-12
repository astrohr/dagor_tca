#!/usr/bin/env python

from __future__ import division

import unittest

from jsonschema import ValidationError
from mock import patch

import track


class TestTracking(unittest.TestCase):
    patches = []

    @classmethod
    def setUpClass(cls):
        cls.patches.append(patch.object(track, 'dagor_motors'))
        for p in cls.patches:
            p.start()

    @classmethod
    def tearDownClass(cls):
        for p in cls.patches:
            p.stop()

    def setUp(self):
        self.tracking = track.Tracking()

    @patch('position.get_chirality')
    @patch('position.get_celest')
    def test_empty_config(self, get_celest, get_chirality):
        get_celest.return_value = {
            'ra': 12.3456,
            'de': 34.5678,
        }
        get_chirality.return_value = 'cw'
        config = {}
        expected_config = track.Tracking.DEFAULT_CONF.copy()
        expected_config.update(
            chirality='cw',
            target_celest={'ra': 12.3456, 'de': 34.5678},
        )
        self.tracking.set_config(config)
        self.assertEqual(self.tracking.config, expected_config)
        get_celest.assert_called_once_with()
        get_chirality.assert_called_once_with()

    @patch('position.get_chirality')
    @patch('position.get_celest')
    def test_invalid_config(self, get_celest, get_chirality):
        get_celest.return_value = {
            'ha': 12.3456,
            'de': 34.5678,
        }
        get_chirality.return_value = 'cw'
        config = {
            'target_celest': {
                'ra': 13.4567,
                'de': -10.987,
            },
            'chirality': 'foobar',
            'stop_on_target': False,
            'rough': False,
        }
        with self.assertRaises(ValidationError):
            self.tracking.set_config(config)
        get_celest.assert_not_called()
        get_chirality.assert_not_called()

    @patch('position.get_chirality')
    @patch('position.get_celest')
    def test_valid_config(self, get_celest, get_chirality):
        get_celest.return_value = {
            'ra': 12.3456,
            'de': 34.5678,
        }
        get_chirality.return_value = 'cw'
        config = {
            'target_celest': None,
            'chirality': None,
            'target_is_static': False,
            'stop_on_target': False,
            'rough': False,
        }
        self.tracking.set_config(config)
        get_celest.assert_called_once_with()
        get_chirality.assert_called_once_with()

