import logging
logger = logging.getLogger('key_transitions')

# Not intended to modify directly
_kt = {
    "linear": [
        9/132, 4/132, 6/132, 6/132, 5/132, 8/132,
        1/132, 8/132, 5/132, 6/132, 6/132, 4/132,
        8/132, 2/132, 7/132, 3/132, 7/132, 7/132,
        3/132, 7/132, 2/132, 8/132, 5/132, 5/132,
    ],
    "exponential2": [
        256/1245, 8/1245, 32/1245, 32/1245, 16/1245, 128/1245,
        1/1245, 128/1245, 16/1245, 32/1245, 32/1245, 8/1245,
        128/1245, 2/1245, 64/1245, 4/1245, 64/1245, 64/1245,
        4/1245, 64/1245, 2/1245, 128/1245, 16/1245, 16/1245,
    ],
    "exponential10": [
        100000000/144442221, 1000/144442221, 100000/144442221,
        100000/144442221, 10000/144442221, 10000000/144442221,
        1/144442221, 10000000/144442221, 10000/144442221,
        100000/144442221, 100000/144442221, 1000/144442221,
        10000000/144442221, 10/144442221, 1000000/144442221,
        100/144442221, 1000000/144442221, 1000000/144442221,
        100/144442221, 1000000/144442221, 10/144442221,
        10000000/144442221, 10000/144442221, 10000/144442221
    ]
}

_ratios = {
    "exponential2": 2,
    "exponential10": 10
}

def insert_new(name, kt):
    kt_sum = sum(kt)
    if 1.0 - abs(kt_sum) > 0.01:
        logger.warn('{} sum is {} when it is expected to be 1.0'.format(name, kt_sum))
    _kt[name] = kt
    logger.info('New key transition {}: {}'.format(name, kt))

def store_ratio(name, ratio):
    _ratios[name] = ratio
    logger.info('New ratio {}: {}'.format(name, ratio))

def get(name):
    if name in _kt:
        return _kt[name]
    else:
        logger.error('{} key transition not found in the dictionary')
        return None

def get_ratio(name):
    if name in _ratios:
        return _ratios[name]
    else:
        logger.error('{} kt ratio not found in the dictionary')
        return None

def get_as_string(name):
    kt = get(name)
    if kt:
        kt_string = ['{:.15f}'.format(p) for p in kt]
        kt_string = ' '.join(kt_string)
        return kt_string
    return None
