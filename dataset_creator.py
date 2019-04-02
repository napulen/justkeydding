from optimizer import ensembler
import collections
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

class DatasetCreator():
    keys_single_letter = {
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
    keys_duple = {
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
    def __init__(self, dataset_folder):
        self.logger = logging.getLogger('DatasetCreator')
        self.logger.info('DatasetCreator() <- dataset_folder={}'.format(dataset_folder))
        self.dataset_okay = False
        self.has_features = False
        # Check critical stuff:
        # dataset dir
        if not os.path.isdir(dataset_folder):
            self.logger.error('The dataset provided is not a folder')
            return dataset_okay
        self.dataset_folder = dataset_folder
        self.feature_folder = os.path.join(dataset_folder, 'features')
        # features folder
        if not os.path.isdir(self.feature_folder):
            self.logger.error('The dataset folder must contain a "features" folder')
            return dataset_okay
        self.files = []
        self.files_no_extension = []
        # files there and right formats
        for f in os.listdir(self.feature_folder):
            self.logger.warning(f)
            tokens = f.rsplit('.', 1)
            if tokens[1] not in ['mid', 'csv', 'wav']:
                self.logger.error('Unknown file type {}. Expecting .wav, .csv, or .mid'.format(f))
                return dataset_okay
            no_extension = tokens[0]
            self.files.append(f)
            self.files_no_extension.append(no_extension)
        if not self.files:
            self.logger.error('The features folder seems to be empty.')
            return dataset_okay
        # Now, not so critical stuff
        self.annotation_folder = os.path.join(dataset_folder, 'annotations')
        if not os.path.isdir(self.annotation_folder):
            self.logger.warning('The dataset folder does not contain annotations, assuming this was expected')
            self.annotation_folder = None
        # And not critical at all
        self.splits_folder = os.path.join(dataset_folder, 'splits')
        if not os.path.isdir(self.splits_folder):
            self.logger.info('No data splits provided. Working on the entire dataset')
            self.splits_folder = None
        self.features = collections.OrderedDict()
        self.annotations = collections.OrderedDict()
        self.splits = {}
        if self.splits_folder:
            for f in os.listdir(self.splits_folder):
                tokens = f.rsplit('.', 1)
                filepath = os.path.join(self.splits_folder, f)
                if len(tokens) < 2 or tokens[1] not in ['txt']:
                    self.logger.warning('Expected {} to be a .txt file.'.format(filepath))
                no_extension = tokens[0]
                with open(filepath, newline='') as fd:
                    split = fd.readlines()
                    self.splits[no_extension] = split
                self.logger.info('Found the following splits: {}'.format(self.splits))
        # If we are here, everything should be okay
        self.dataset_okay = True
    
    def parse_label(self, annotation):
        annotation = annotation.strip().split()
        if len(annotation) == 1:
            key = annotation[0]
            if key in DatasetCreator.keys_single_letter:
                label = DatasetCreator.keys_single_letter[key]
        elif len(annotation) == 2:
            key = (annotation[0].lower(), annotation[1].lower())
            if key in DatasetCreator.keys_duple:
                label = DatasetCreator.keys_duple[key]
        else:
            self.logger.error('Did not recognize the format of label {}'.format(annotation))
            label = -1
        return label

    def compute_features(self, key_profiles, key_transitions, mixed_profiles=False):
        if not self.dataset_okay:
            return
        ens = ensembler.Ensembler(key_profiles, key_transitions)
        for i in range(len(self.files)):
            feature_filepath = os.path.join(self.feature_folder, self.files[i])
            features = ens.evaluate(feature_filepath, mixed_profiles)
            features = [f for l in features for f in l]
            feature_array = np.array(features)
            self.features[self.files_no_extension[i]] = feature_array
            if self.annotation_folder:
                annotation_name = '{}.key'.format(self.files_no_extension[i])
                annotation_filepath = os.path.join(self.annotation_folder, annotation_name)
                if not os.path.exists(annotation_filepath):
                    self.logger.error('Could not find file {}'.format(annotation_filepath))
                    return
                with open(annotation_filepath, newline='') as fd:
                    annotation_data = fd.readlines()[0]
                label = self.parse_label(annotation_data)
                self.annotations[self.files_no_extension[i]] = label
        self.has_features = True

    def write(self, output_dir, dataset_name):
        self.logger.info('write() <- output_dir={}, dataset_name={}'.format(output_dir, dataset_name))
        if not self.has_features:
            return
        if not os.path.exists(output_dir):
            self.logger.warning('Output folder {} does not exist. Trying to create it'.format(output_dir))
            os.makedirs(output_dir)
            self.logger.warning('Success.')
        features_filename = '{}_features.pkl'.format(dataset_name)
        feature_filepath = os.path.join(output_dir, features_filename)
        feature_array = list(self.features.values())
        feature_array = np.array(feature_array)
        self.logger.info('writing {}'.format(feature_filepath))
        feature_array.dump(feature_filepath)
        if self.annotations:
            annotation_filename = '{}_annotations.pkl'.format(dataset_name)
            annotation_filepath = os.path.join(output_dir, annotation_filename)
            annotation_array = list(self.annotations.values())
            annotation_array = np.array(annotation_array)
            self.logger.info('writing {}'.format(annotation_filepath))
            annotation_array.dump(annotation_filepath)
        return

if __name__ == '__main__':
    if not os.path.exists('logs'):
        os.makedirs('logs')
    logging.config.dictConfig(logging_dict)
    logger = logging.getLogger('dataset_creator')
    if len(sys.argv) != 2 or not os.path.isdir(sys.argv[1]):
        logger.error('You need to provide a dataset')
        exit()
    dataset = sys.argv[1]
    key_profiles = ['krumhansl_kessler', 'aarden_essen', 'sapp', 'bellman_budge', 'temperley', 'albrecht_shanahan2']
    key_transitions = ['ktg_exponential5', 'ktg_exponential10', 'ktg_exponential15']
    dc = DatasetCreator(dataset)
    dc.compute_features(key_profiles, key_transitions)
    dc.write('datasets', 'test_dataset')
