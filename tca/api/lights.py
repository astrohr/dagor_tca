from flask import Blueprint, render_template, abort, request
import json
from tca import lights as dagor_lights


DEFAULT_PREFIX = '/lights'

api = Blueprint('lights', __name__)


@api.route('/', methods=['GET', 'POST', 'PUT'])
def main_resource():
    if request.method in {'POST', 'PUT', }:
        data = request.get_json(force=True)
        n = data['n']
        dagor_lights.set_lights(n)
    return json.dumps({
        'n': dagor_lights.get_lights(),
    })


if __name__ == '__main__':
    app = Flask(__name__)
    app.debug = True
    app.register_blueprint(lights_api, url_prefix=DEFAULT_PREFIX)
    app.run("0.0.0.0")
