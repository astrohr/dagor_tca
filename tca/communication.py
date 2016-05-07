#!/usr/bin/env python
""" REST api service """
import json
from flask import Flask
from flask import request
from formats import parse_degrees, parse_hours
import position as dagor_position
import cat as dagor_cat
import lights as dagor_lights

app = Flask(__name__)
TRACKING_COORDINATES_FILE = 'coords.txt'


def target_repr():
    try:
        with open(TRACKING_COORDINATES_FILE) as f:
            rascension, declination = f.readline().split(" ")
            rascension = parse_hours(rascension)
            declination = parse_degrees(declination)
            coords = {
                "ra" : rascension,
                "de" : declination,
            }
            return json.dumps(coords)
    except(IOError, ValueError):
        return ""


@app.route('/target', methods=['GET', 'PUT'])
def target():

    if request.method == 'GET':
        return target_repr()

    elif request.method == 'PUT':
        coords = json.loads(request.data)
        line = str(coords["ra"]) + " " + str(coords["de"])
        try:
            f = open(TRACKING_COORDINATES_FILE, 'w')
            f.write(line)
            f.close()
        except Exception as e:
            print e
        return target_repr()


@app.route('/delta', methods=['PUT'])
def putdelta():

    try:
        coords = json.loads(request.data)
        f = open(TRACKING_COORDINATES_FILE, 'w')
        current = dagor_position.get_celest()
        rascension = current['ra'] + parse_hours(str(coords['ra']))
        declination = current['de'] + parse_degrees(str(coords['de']))
        if declination > 90:
            declination = 180 - declination
            rascension = rascension + 12
        while rascension >= 24:
            rascension = rascension - 24
        while rascension < 0:
            rascension = 24 - rascension

        f.write(str(rascension) + " " + str(declination))
        f.close()
    except Exception as e:
        print e
        return str(e)
    return "ok"


@app.route('/status', methods=['GET'])
def getstatus():

    current = {
        "celest" : dagor_position.get_celest(),
        "local" : dagor_position.get_local(),
        "altaz" : dagor_position.get_altaz(),
    }
    return json.dumps(current)


@app.route('/cat', methods=['GET'])
def getcat():

    return json.dumps({
        'catalog': dagor_cat.dump(),
    })


@app.route('/lights', methods=['GET', 'PUT',])
def lights():
    if request.method == 'PUT':
        try:
            state = json.loads(request.data.strip())['lights']
            dagor_lights.set_lights(state)
        except Exception as e:
            print e
            return str(e)
    # GET (always):
    return json.dumps({
        'lights': dagor_lights.get_lights(),
    })







if __name__ == '__main__':
    app.run(host='0.0.0.0')
