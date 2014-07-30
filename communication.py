
import json
from flask import Flask
from flask import request
from formats import parse_degrees, parse_hours
import position as dagor_position

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
        line = str(coords["de"]) + " " + str(coords["ra"])
        try:
            f = open(TRACKING_COORDINATES_FILE, 'w')
            f.write(line)
            f.close()
        except Exception as e:
            print e
        return target_repr()


@app.route('/delta', methods=['PUT'])
def putdelta():

    coords = json.loads(request.data)
    try:
        f = open(TRACKING_COORDINATES_FILE, 'r+')
        rascension, declination = f.readline().split(" ")
        rascension = parse_hours(rascension) + parse_hours(coords['ra'])
        declination = parse_degrees(declination) + parse_degrees(coords['de'])
        f.write(str(rascension) + " " + str(declination))
        f.close()
    except Exception as e:
        print e
    return "ok"


@app.route('/status', methods=['GET'])
def getstatus():

    current = {
        "celest" : dagor_position.get_celest(),
        "local" : dagor_position.get_local(),
        "altaz" : dagor_position.get_altaz(),
    }
    return json.dumps(current)





if __name__ == '__main__':
    app.run(host='0.0.0.0')