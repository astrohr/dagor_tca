from flask.ext.api import renderers


class TextRenderer(renderers.BaseRenderer):
    media_type = 'application/text'

    def render(self, data, media_type, **options):
        return data


class BrowsableAPITextRenderer(renderers.BrowsableAPIRenderer):
    template = 'base_text.html'
