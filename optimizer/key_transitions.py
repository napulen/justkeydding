import logging
logger = logging.getLogger('key_transitions')

# Not intended to modify directly
_kt = {
    "ktg_linear": [
        9/132, 4/132, 6/132, 6/132, 5/132, 8/132,
        1/132, 8/132, 5/132, 6/132, 6/132, 4/132,
        8/132, 2/132, 7/132, 3/132, 7/132, 7/132,
        3/132, 7/132, 2/132, 8/132, 5/132, 5/132,
    ],
    "ktg_exponential2": [
        256/1245, 8/1245, 32/1245, 32/1245, 16/1245, 128/1245,
        1/1245, 128/1245, 16/1245, 32/1245, 32/1245, 8/1245,
        128/1245, 2/1245, 64/1245, 4/1245, 64/1245, 64/1245,
        4/1245, 64/1245, 2/1245, 128/1245, 16/1245, 16/1245,
    ],
    "ktg_exponential10": [
        100000000/144442221, 1000/144442221, 100000/144442221,
        100000/144442221, 10000/144442221, 10000000/144442221,
        1/144442221, 10000000/144442221, 10000/144442221,
        100000/144442221, 100000/144442221, 1000/144442221,
        10000000/144442221, 10/144442221, 1000000/144442221,
        100/144442221, 1000000/144442221, 1000000/144442221,
        100/144442221, 1000000/144442221, 10/144442221,
        10000000/144442221, 10000/144442221, 10000/144442221
    ],
    "ktg_experiment6": [
        0.8765246313562368, 3.993824473043542e-08, 3.450952918589394e-05, 3.450952918589394e-05, 1.1739889361311444e-06, 0.02981872670344273, 1.5724030710075827e-12, 0.02981872670344273, 1.1739889361311444e-06, 3.450952918589394e-05, 3.450952918589394e-05, 3.993824473043542e-08,

        0.02981872670344273, 4.622095490077414e-11, 0.0010144112674150716, 1.3586698673708549e-09, 0.0010144112674150716, 0.0010144112674150716, 1.3586698673708549e-09, 0.0010144112674150716, 4.622095490077414e-11, 0.02981872670344273, 1.1739889361311444e-06, 1.1739889361311444e-06
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

def flush_key_transitions(key_transitions):
    [_kt.pop(kt) for kt in key_transitions]
    [_ratios.pop(r) for r in key_transitions if r in _ratios]
    logger.info('Removed the following key transitions: {}'.format(key_transitions))

def keep_key_transitions(key_transitions):
    [_kt.pop(kt) for kt in list(_kt.keys()) if kt not in key_transitions]
    [_ratios.pop(r) for r in list(_ratios.keys()) if r not in key_transitions]
    logger.info('Removed all but the following key transitions: {}'.format(key_transitions))

def log_key_transitions_dict():
    logger.info('Current key transitions:')
    for k in _kt:
        logger.info('{}: {}'.format(k, get_as_string(k)))
        if k in _ratios:
            logger.info('ratio={}'.format(_ratios[k]))

def get(name):
    if name in _kt:
        return _kt[name]
    else:
        logger.error('{} key transition not found in the dictionary'.format(name))
        return None

def get_ratio(name):
    if name in _ratios:
        return _ratios[name]
    else:
        logger.error('{} kt ratio not found in the dictionary'.format(name))
        return None

def get_as_string(name):
    kt = get(name)
    if kt:
        kt_string = ['{:.15f}'.format(p) for p in kt]
        kt_string = ' '.join(kt_string)
        return kt_string
    return None
