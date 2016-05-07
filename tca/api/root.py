#!/usr/bin/env python
from flask import Flask
import lights


app = Flask(__name__)
app.debug = True
app.register_blueprint(lights.api, url_prefix=lights.DEFAULT_PREFIX)


@app.route('/')
def hello_world():
    return 'Hello World!'

if __name__ == '__main__':
    app.run("0.0.0.0", 8001)
