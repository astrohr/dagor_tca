# coding=utf-8
from collections import OrderedDict

from flask_api.decorators import set_renderers

from flask_api.renderers import JSONRenderer

from flask_api import FlaskAPI

VERSION = (0, 2, 0)
version = ".".join(map(str, VERSION))

from . import focus, lights
from .utils import RegexConverter, BrowsableAPITitleRenderer


MODULES = (
    lights,
    focus
)


app = FlaskAPI(__name__)
app.debug = False
app.url_map.converters['regex'] = RegexConverter

for module in MODULES:
    app.register_blueprint(module.api, url_prefix=module.DEFAULT_PREFIX)


@set_renderers(BrowsableAPITitleRenderer, JSONRenderer)
def dagor_api():
    """
    API root

    version: {VERSION}

    ### Hyperlinks

    {modules}

    """
    value = OrderedDict()
    value['version'] = version
    return value


modules = []
for module in MODULES:
    modules.append(
        ' * [{name}]({url}/)'.format(
            name=module.api.name,
            url=module.DEFAULT_PREFIX,
        )
    )
dagor_api.__doc__ = dagor_api.__doc__.format(
    VERSION=version,
    modules='\n    '.join(modules)
)
dagor_api.__name__ = "Dagor API"
app.route('/')(dagor_api)


def run():
    app.run("0.0.0.0", 8001)
