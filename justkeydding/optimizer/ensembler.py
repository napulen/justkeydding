from justkeydding.parameters import key_profiles, key_transitions
import subprocess
import logging
import itertools
from multiprocessing.dummy import Pool as ThreadPool

class Ensembler:
    def __init__(self, profiles, transitions):
        self.logger = logging.getLogger('ensembler')
        self.logger.info('Ensembler() <- profiles={}, transitions={}'.format(profiles, transitions))
        self.profiles = profiles
        self.transitions = transitions

    def grade_key_profiles(self, key_profiles, key_transition_name):
        ''' Grade a list of key profiles '''
        self.logger.info('Start grade_key_profiles() <- key_profiles={}, key_transition_name={}'.format(key_profiles, key_transition_name))
        grading = [self.evaluate(x, key_transition_name) for x in key_profiles]
        grading = sorted(grading, key=lambda score: score[0])
        self.logger.info('Done grade_key_profiles() -> grading={}'.format(grading))
        return grading

    def grade_key_transitions(self, key_profile_name, key_transitions):
        ''' Grade a list of key transitions '''
        self.logger.info('Start grade_key_transitions() <- key_profile_name={}, key_transitions ={}'.format(key_profile_name, key_transitions))
        grading = [self.evaluate(key_profile_name, x) for x in key_transitions]
        grading = sorted(grading, key=lambda score: score[0])
        self.logger.info('Done grade_key_transitions() -> grading={}'.format(grading))
        return grading

    def get_ensemble(self, mixed_profiles=False):
        ''' Create the ensemble '''
        self.logger.info('Start get_ensemble() <- mixedProfiles={}'.format(mixed_profiles))
        if mixed_profiles:
            profiles = [key_profiles.mix(m[0], m[1]) for m in itertools.product(self.profiles, self.profiles)]
        else:
            profiles = self.profiles
        self.ensemble = [(e[0], e[1]) for e in itertools.product(profiles, self.transitions)]
        return self.ensemble

    def evaluate(self, filename, mixed_profiles=False):
        ''' Evaluate a key profile '''
        self.logger.info('Start evaluate() <- filename={}'.format(filename))
        if not hasattr(self, 'ensemble'):
            self.get_ensemble(mixed_profiles)
        with ThreadPool(4) as p:
            features = p.map(lambda e: self.run_keydetection(filename, e), self.ensemble)
        self.logger.info('Done evaluate() -> features={}'.format(features))
        return features

    def run_keydetection(self, filename, ensemble):
        self.logger.debug('run_keydetection() <- filename={}, ensemble={}'.format(filename, ensemble))
        key_profile_name = ensemble[0]
        key_transition_name = ensemble[1]
        kp_string = key_profiles.get_as_string(key_profile_name)
        kt_string = key_transitions.get_as_string(key_transition_name)
        justkeydding = subprocess.Popen(
                ('bin/justkeydding',
                '-p',
                '-K',
                '{}'.format(kp_string),
                '-T',
                '{}'.format(kt_string),
                filename),
                stdout=subprocess.PIPE)
        output, _ = justkeydding.communicate()
        features = output.split()
        if len(features) != 24:
            self.logger.error('Failed while running justkeydding, output: {}'.format(output))
            features = ['-1000000'] * 24
        features = [float(x) for x in features]
        self.logger.debug('{} {}: {}'.format(filename, ensemble, features))
        return features
