
from flask import Flask
from flask import request
from formats import parse_degrees, parse_hours

app = Flask(__name__)

@app.route('/position', methods=['GET', 'PUT'])
def Coordinate():
    print request.method
    if request.method == 'GET':
        TRACKING_COORDINATES_FILE = 'coords.txt'
        try:
            with open(TRACKING_COORDINATES_FILE) as f:
                rascension, declination = f.readline().split(" ")
                rascension = parse_hours(rascension)
                declination = parse_degrees(declination)
            return "(rascension, declination)"
        except(IOError, ValueError):
            return ""

    elif request.method == 'PUT':
        #f = open('coords.txt','w')
        #f.write(rascension, declination) # python will convert \n to os.linesep
        #f.close() # you can omit in most cases as the destructor will call if
        return ""


if __name__ == '__main__':
    app.run(host='0.0.0.0')