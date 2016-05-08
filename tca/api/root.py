#!/usr/bin/env python
from flask_api import FlaskAPI
import lights

app = FlaskAPI(__name__)
app.debug = True
app.register_blueprint(lights.api, url_prefix=lights.DEFAULT_PREFIX)


@app.route('/')
def hello_world():
    return 'Hello World!'

if __name__ == '__main__':
    app.run("0.0.0.0", 8001)
