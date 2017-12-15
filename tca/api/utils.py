# coding=utf-8
import os
from functools import wraps

from retrying import retry

from flask.ext.api import renderers, parsers, exceptions, status as http_status
from flask.ext.api.decorators import set_renderers
from flask.ext.api.renderers import BrowsableAPIRenderer, dedent, html_escape
from flask_api.exceptions import ParseError
from flask_api.mediatypes import MediaType
from flask_api.compat import apply_markdown
from flask import request, render_template, current_app
# noinspection PyProtectedMember
from flask.globals import _request_ctx_stack
from werkzeug.routing import BaseConverter

from tca.logging_conf import get_logger


logger = get_logger('api.utils')


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


class BrowsableAPITitleRenderer(BrowsableAPIRenderer):
    media_type = 'text/html'
    handles_empty_responses = True
    template = 'base.html'

    def render(self, data, media_type, **options):
        """ Just... Don't capitalize view_name! """

        # noinspection PyUnresolvedReferences
        available_renderers = [
            renderer for renderer in request.renderer_classes
            if not issubclass(renderer, BrowsableAPIRenderer)
        ]
        assert available_renderers, \
            'BrowsableAPIRenderer cannot be the only renderer'
        mock_renderer = available_renderers[0]()
        mock_media_type = MediaType(mock_renderer.media_type)
        if data == '' and not mock_renderer.handles_empty_responses:
            mock_content = None
        else:
            mock_content = mock_renderer.render(data, mock_media_type, indent=4)

        # Determine the allowed methods on this view.
        adapter = _request_ctx_stack.top.url_adapter
        allowed_methods = adapter.allowed_methods()

        endpoint = request.url_rule.endpoint
        view_name = str(endpoint)
        view_description = current_app.view_functions[endpoint].__doc__
        if view_description is not None:
            view_description = dedent(view_description)
        mock_content = html_escape(mock_content)

        if view_description and apply_markdown:
            view_description = apply_markdown(view_description)

        status = options['status']
        headers = options['headers']
        headers['Content-Type'] = str(mock_media_type)

        from flask_api import __version__

        context = {
            'status': status,
            'headers': headers,
            'content': mock_content,
            'allowed_methods': allowed_methods,
            'view_name': view_name,
            'view_description': view_description,
            'version': __version__
        }
        return render_template(self.template, **context)


class BoolRenderer(renderers.JSONRenderer):
    """JSON renderer that only supports "true" and "false" values"""
    media_type = 'application/json'

    def render(self, data, media_type, **options):
        try:
            return "true" if data in {"true", "True"} else "false"
        except:
            return super(BoolRenderer, self).render(data, media_type, **options)


class BoolBrowsableAPIRenderer(renderers.BrowsableAPIRenderer):
    media_type = 'text/html'

    def render(self, data, media_type, **options):
        data = "true" if data == "true" else "false"
        return super(BoolBrowsableAPIRenderer, self).render(
            data, media_type, **options)


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
        # noinspection PyBroadException
        try:
            return '{}'.format(int(data))
        except:
            return super(IntRenderer, self).render(data, media_type, **options)


class IntBrowsableAPIRenderer(renderers.BrowsableAPIRenderer):
    media_type = 'text/html'

    def render(self, data, media_type, **options):
        if not type(data) == dict:
            data = '{}'.format(data)
        return super(IntBrowsableAPIRenderer, self).render(
            data, media_type, **options)


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
    }, http_status.HTTP_400_BAD_REQUEST


def set_mock_var():
    os.environ['DAGOR_API_MOCK'] = '1'


def read_mock_var():
    return os.environ.get('DAGOR_API_MOCK')


# decorators


def set_logger(logger):

    def _set_logger(func):
        func.logger = logger
        return func

    return _set_logger


def check_connectivity(func):
    @wraps(func)
    def func_wrapper(*args_, **kwargs_):
        try:
            return func(*args_, **kwargs_)
        except Exception as e:
            logger.exception('API - connection failure')
            return {
                'ready': False,
                'message': '{}'.format(e),
            }, http_status.HTTP_503_SERVICE_UNAVAILABLE
    return func_wrapper


def handle_request_errors(func):
    @wraps(func)
    def func_wrapper(*args_, **kwargs_):
        try:
            return func(*args_, **kwargs_)
        except ParseError as e:
            return render_error(e)
    return func_wrapper


def yes_just_log_it(exc):
    if isinstance(exc, KeyboardInterrupt):
        logger.debug('API - interrupted, {}'.format(repr(exc)))
        return False
    logger.warning('API - retrying, {}'.format(repr(exc)))
    return True


retry_serial = retry(
    wait_fixed=20,  # ms
    stop_max_delay=300,  # ms
    retry_on_exception=yes_just_log_it,
)
