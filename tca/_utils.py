from functools import wraps


def str_bool(value):
    """
    Parse bool from string

    Usage:
    >>> str_bool("1")
    True
    >>> str_bool("0")
    False
    >>> str_bool("TRUE")
    True
    >>> str_bool("false")
    False

    :param value: str
    :return: bool
    """
    value = value.strip().lower()
    if value in {'0', 'false'}:
        return False
    return True
