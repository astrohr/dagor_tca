from flask import Blueprint, render_template, abort, request
from flask.ext.api import FlaskAPI, status
from tca import lights as dagor_lights


DEFAULT_PREFIX = '/lights'

api = Blueprint('lights', __name__)


def device_repr():
    return {
        'n': dagor_lights.get_lights(),
    }


@api.route('/', methods=['GET', 'POST', 'PUT'])
def main_resource():
    if request.method in {'POST', 'PUT', }:
        n = request.data['n']
        dagor_lights.set_lights(n)
        return device_repr(), status.HTTP_202_ACCEPTED
    return device_repr()


if __name__ == '__main__':
    app = FlaskAPI(__name__)
    app.debug = True
    app.register_blueprint(api, url_prefix=DEFAULT_PREFIX)
    app.run("0.0.0.0")
