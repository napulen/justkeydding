from optimizer import ensembler
import optimizer.key_profiles
import optimizer.key_transitions
import os
import logging
import logging.handlers
import logging.config
import numpy as np
import re

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

keys ={
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

def ensemble_runner(dataset, initial_key_profiles, initial_key_transitions, mixed_profiles=False):
    logger.info('ensemble_runner() <- dataset={}, initial_key_profiles={}, initial_key_transitions={}, mixed_profiles={}'.format(dataset, initial_key_profiles, initial_key_transitions, mixed_profiles))
    ens = ensembler.Ensembler(initial_key_profiles, initial_key_transitions)
    dataset_features = []
    labels = []
    for filename in dataset:
        try:
            ground_truth = filename.rsplit('_', 1)[-1].split('.')[0]
        except:
            ground_truth = 'None'
        logger.info('ground_truth: {}'.format(ground_truth))
        label = keys[ground_truth] if ground_truth in keys else -1
        labels.append(label)
        features = ens.evaluate(filename, mixed_profiles)
        features = [f for l in features for f in l]
        feature_array = np.array(features)
        dataset_features.append(feature_array)
        logger.info(feature_array)
    np.array(dataset_features).dump('dataset_features.pkl')
    np.array(labels).dump('dataset_labels.pkl')

if __name__ == '__main__':
    if not os.path.exists('logs'):
        os.makedirs('logs')
    logging.config.dictConfig(logging_dict)
    logger = logging.getLogger('ensemble_runner')
    with open('midi_dataset.txt') as f:
        dataset = f.readlines()
        dataset = [x.strip() for x in dataset]
    initial_key_profiles = ['sapp', 'temperley', 'krumhansl_kessler']
    initial_key_transitions = ['ktg_exponential10', 'ktg_experiment6', 'ktg_napoles_midi128']
    ensemble_runner(dataset, initial_key_profiles, initial_key_transitions, mixed_profiles=True)
