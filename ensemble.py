from optimizer import ensembler
import optimizer.key_profiles
import optimizer.key_transitions
import os
import logging
import logging.handlers
import logging.config
import numpy as np
import re
import sys

logging_dict = {
    'version': 1,
    'disable_existing_loggers': False,
    'formatters': {
        'date': {
            'format': '%(asctime)s [%(name)-15s] %(levelname)-7s: %(message)s',
        },
        'no_date': {
            'format': '[%(name)-15s] %(levelname)-7s: %(message)s',
        },
    },
    'handlers': {
        'file_handler': {
            'class': 'logging.handlers.TimedRotatingFileHandler',
            'filename': 'logs/optimizer.log',
            'when': 'H',
            'interval': 1,
            'backupCount': 360,
            'formatter': 'date',
            'encoding': 'utf8'
        },
        'console': {
            'class': 'logging.StreamHandler',
            'level': 'DEBUG',
            'formatter': 'no_date'
        }
    },
    'loggers': {
        '': {
            'handlers': ['file_handler', 'console'],
            'level': 'DEBUG',
            'propagate': True
        }
    }
}

keys_format0 = {
    'C': 0, 'C#': 1, 'Db': 1, 'D': 2,
    'D#': 3, 'Eb': 3, 'E': 4, 'F': 5,
    'F#': 6, 'Gb': 6, 'G': 7, 'G#': 8,
    'Ab': 8, 'A': 9, 'A#': 10, 'Bb': 10,
    'B': 11,

    'c': 12, 'c#': 13, 'db': 13, 'd': 14,
    'd#': 15, 'eb': 15, 'e': 16, 'f': 17,
    'f#': 18, 'gb': 18, 'g': 19, 'g#': 20,
    'ab': 20, 'a': 21, 'a#': 22, 'bb': 22,
    'b': 23
}

keys_format1 = {
    ('c', 'major'): 0, ('c#', 'major'): 1,
    ('db', 'major'): 1, ('d', 'major'): 2,
    ('d#', 'major'): 3, ('eb', 'major'): 3,
    ('e', 'major'): 4, ('f', 'major'): 5,
    ('f#', 'major'): 6, ('gb', 'major'): 6,
    ('g', 'major'): 7, ('g#', 'major'): 8,
    ('ab', 'major'): 8, ('a', 'major'): 9,
    ('a#', 'major'): 10, ('bb', 'major'): 10,
    ('b', 'major'): 11,

    ('c', 'minor'): 12, ('c#', 'minor'): 13,
    ('db', 'minor'): 13, ('d', 'minor'): 14,
    ('d#', 'minor'): 15, ('eb', 'minor'): 15,
    ('e', 'minor'): 16, ('f', 'minor'): 17,
    ('f#', 'minor'): 18, ('gb', 'minor'): 18,
    ('g', 'minor'): 19, ('g#', 'minor'): 20,
    ('ab', 'minor'): 20, ('a', 'minor'): 21,
    ('a#', 'minor'): 22, ('bb', 'minor'): 22,
    ('b', 'minor'): 23
}

def parse_label(annotation_data):
    annotation_data = annotation_data.strip()
    annotation_data = annotation_data.split()
    if len(annotation_data) == 1:
        if annotation_data in keys_format0:
            ground_truth = keys_format0[annotation_data]
    elif len(annotation_data) == 2:
        if annotation_data in keys_format1:
            ground_truth = keys_format1[annotation_data]
    else:
        logger.error('Did not recognize the format of label {}'.format(annotation_data))
        ground_truth = -1
    return ground_truth

def ensemble_runner(dataset, initial_key_profiles, initial_key_transitions, mixed_profiles=False):
    logger.info('ensemble_runner() <- dataset={}, initial_key_profiles={}, initial_key_transitions={}, mixed_profiles={}'.format(dataset, initial_key_profiles, initial_key_transitions, mixed_profiles))
    ens = ensembler.Ensembler(initial_key_profiles, initial_key_transitions)
    dataset_features = []
    labels = []
    features_dir = os.path.join(dataset, 'features')
    annotations_dir = os.path.join(dataset, 'annotations')
    # Verifying the dataset structure
    if not os.path.isdir(features_dir):
        logger.error('The dataset dir should contain a "features" folder with all the data')
        return
    else:
        features = os.listdir(features_dir)
    if not os.path.isdir(annotations_dir):
        logger.warn('There is no "annotations" folder in dataset {}, assuming only the features are required'.format(dataset))
        search_labels = False
    else:
        search_labels = True
    # Start computing every file
    for filename in features:
        label = -1
        if search_labels:
            annotation = '{}.key'.format(filename[:-4])
            annotation_file = os.path.join(annotations_dir, annotation)
            if not os.path.exists(annotation_file):
                logger.warn('Could not find the annotation for {}'.format(filename))
            else:
                with open(annotation_file, newline='') as f:
                    annotation_data = f.readlines()[0]
                label = parse_label(annotation_data)
        logger.info('label: {}'.format(label))
        labels.append(label)
        features = ens.evaluate(os.path.join(features_dir, filename), mixed_profiles)
        features = [f for l in features for f in l]
        feature_array = np.array(features)
        dataset_features.append(feature_array)
        logger.info(feature_array)
    np.array(dataset_features).dump('{}_features.pkl'.format(dataset))
    np.array(labels).dump('{}_labels.pkl'.format(dataset))

if __name__ == '__main__':
    if not os.path.exists('logs'):
        os.makedirs('logs')
    logging.config.dictConfig(logging_dict)
    logger = logging.getLogger('ensemble_runner')
    if len(sys.argv) != 2 or not os.path.isdir(sys.argv[1]):
        logger.error('You need to provide a dataset')
        exit()
    dataset = sys.argv[1]
    initial_key_profiles = ['krumhansl_kessler', 'aarden_essen', 'sapp', 'bellman_budge', 'temperley', 'albrecht_shanahan2']
    initial_key_transitions = ['ktg_exponential5', 'ktg_exponential10', 'ktg_exponential15']
    ensemble_runner(dataset, initial_key_profiles, initial_key_transitions, mixed_profiles=False)
