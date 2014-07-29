
import json
from flask import Flask
from flask import request
from formats import parse_degrees, parse_hours

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
    print request.method
    if request.method == 'GET':
        return target_repr()

    elif request.method == 'PUT':
        coords = json.loads(request.data)
        line = str(coords["de"]) + " " + str(coords["ra"])
        print line
        try:
            f = open(TRACKING_COORDINATES_FILE, 'w')
            f.write(line)
            f.close()
        except Exception as e:
            print e
        return target_repr()


if __name__ == '__main__':
    app.run(host='0.0.0.0')