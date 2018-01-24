# coding=utf-8
import json
from textwrap import dedent

import jsonschema

SCHEMAS = {
    'track': dedent('''
        {
          "$id": "dagor_tca_track_conf",
          "type": "object",
          "definitions": {},
          "$schema": "dagor_tca_track_conf",
          "minProperties": 5,
          "properties": {
            "target_celest": {
              "$id": "/properties/target_celest",
              "type": ["object", "null"],
              "oneOf": [
                {
                  "properties": {
                    "ra": {
                      "$id": "/properties/target_celest/properties/ra",
                      "type": "number",
                      "title": "The Ra Schema",
                      "description": "An explanation about the purpose of this instance.",
                      "default": 0,
                      "examples": [
                        1.2345600128173828
                      ],
                      "minimum": 0,
                      "maximum": 24
                    },
                    "de": {
                      "$id": "/properties/target_celest/properties/de",
                      "type": "number",
                      "title": "The De Schema",
                      "description": "An explanation about the purpose of this instance.",
                      "default": 0,
                      "examples": [
                        12.34533977508545
                      ],
                      "minimum": -90,
                      "maximum": 90
                    }
                  },
                  "required": [
                    "ra",
                    "de"
                  ]
                },
                {
                  "type": "null"
                }
              ]
            },
            "chirality": {
              "$id": "/properties/chirality",
              "type": [
                "string",
                "null"
              ],
              "title": "The Chirality Schema",
              "description": "An explanation about the purpose of this instance.",
              "default": "",
              "examples": [
                "cw"
              ],
              "enum": [
                "ce",
                "cw",
                "cc"
              ]
            },
            "target_is_static": {
              "$id": "/properties/target_is_static",
              "type": "boolean",
              "title": "The Target_is_static Schema",
              "description": "An explanation about the purpose of this instance.",
              "default": false,
              "examples": [
                false
              ]
            },
            "stop_on_target": {
              "$id": "/properties/stop_on_target",
              "type": "boolean",
              "title": "The Stop_on_target Schema",
              "description": "An explanation about the purpose of this instance.",
              "default": false,
              "examples": [
                false
              ]
            },
            "rough": {
              "$id": "/properties/rough",
              "type": "boolean",
              "title": "The Rough Schema",
              "description": "An explanation about the purpose of this instance.",
              "default": false,
              "examples": [
                false
              ]
            },
            "force": {
              "$id": "/properties/force",
              "type": "boolean",
              "title": "The Force Schema",
              "description": "An explanation about the purpose of this instance.",
              "default": false,
              "examples": [
                false
              ]
            },
            "tracking": {
              "$id": "/properties/tracking",
              "type": "boolean",
              "title": "The Tracking Schema",
              "description": "An explanation about the purpose of this instance.",
              "default": false,
              "examples": [
                false
              ]
            }
          },
          "required": [
            "chirality",
            "target_is_static",
            "stop_on_target",
            "rough",
            "force",
            "tracking"
          ]
        }
        '''),
}


class Schema(object):
    def __init__(self, name):
        self.name = name
        self.json = SCHEMAS[name]
        self.schema = json.loads(self.json)

    def validate(self, data):
        jsonschema.validate(data, self.schema)
