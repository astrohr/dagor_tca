# coding=utf-8
from flask.ext.api import renderers, parsers, exceptions, status
from flask.ext.api.decorators import set_renderers
from werkzeug.routing import BaseConverter


class RegexConverter(BaseConverter):
    """
    Use regex for URL pattern matching.

    Usage:
        @api.route('/<regex("[a-z]{2,3}"):lang_code>/', ...)
        def view(lang_code):
            ...

        app.url_map.converters['regex'] = RegexConverter
    """
    def __init__(self, url_map, *items):
        super(RegexConverter, self).__init__(url_map)
        self.regex = items[0]


class BoolRenderer(renderers.BaseRenderer):
    """JSON renderer that only supports "true" and "false" values"""
    media_type = 'application/json'

    def render(self, data, media_type, **options):
        return "true" if data == "true" else "false"


class BoolBrowsableAPIRenderer(renderers.BrowsableAPIRenderer):
    media_type = 'text/html'

    def render(self, data, media_type, **options):
        data = "true" if data == "true" else "false"
        return super(BoolBrowsableAPIRenderer, self).render(data, media_type, **options)


class BoolParser(parsers.BaseParser):
    """Parser for JSON bool values"""
    media_type = 'application/json'

    def parse(self, stream, media_type, **options):
        data = stream.read().decode('utf-8').lower()
        if data == "false":
            return False
        if data == "true":
            return True
        raise exceptions.ParseError('Invalid request, use "true" or "false"')


class IntRenderer(renderers.JSONRenderer):
    """JSON renderer that only supports integer"""
    media_type = 'application/json'

    def render(self, data, media_type, **options):
        try:
            return '{}'.format(int(data))
        except:
            print(type(data))
            print(data)
            return super(IntRenderer, self).render(data, media_type, **options)


class IntBrowsableAPIRenderer(renderers.BrowsableAPIRenderer):
    media_type = 'text/html'

    def render(self, data, media_type, **options):
        if not type(data) == dict:
            data = '{}'.format(data)
        return super(IntBrowsableAPIRenderer, self).render(data, media_type, **options)


class IntParser(parsers.BaseParser):
    """Parser for JSON int values"""
    media_type = 'application/json'

    def parse(self, stream, media_type, **options):
        data = stream.read().decode('utf-8').lower()
        try:
            data = int(data)
        except ValueError:
            raise exceptions.ParseError('Invalid request, expected integer')
        return data


@set_renderers(renderers.BrowsableAPIRenderer, renderers.JSONRenderer)
def render_error(message):
    """ Render errors in JSON. Also make sure JSONRenderer is set."""
    return {
       'message': '{}'.format(message),
    }, status.HTTP_400_BAD_REQUEST
