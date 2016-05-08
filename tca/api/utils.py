# coding=utf-8
from flask.ext.api import renderers, parsers, exceptions
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
        print("parsing: {}".format(data))
        if data == "false":
            return False
        if data == "true":
            return True
        raise exceptions.ParseError('Invalid request, use "true" or "false"')
