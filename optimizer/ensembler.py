import subprocess
import logging
from . import key_profiles
from . import key_transitions
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

    def evaluate(self, filename):
        ''' Evaluate a key profile '''
        self.logger.info('Start evaluate() <- filename={}'.format(filename))
        # error_list = [self.run_keydetection(filename, kp_string)
        #              for filename in self.files]
        with ThreadPool(max(len(self.files), 8)) as p:
            error_list = p.map(lambda f: self.run_keydetection(f, key_profile_name, key_transition_name), self.files)
        total_error = sum(error_list)
        self.logger.info('Done evaluate() -> total_error={}'.format(total_error))
        return (total_error, key_profile_name, key_transition_name)

    def run_keydetection(self, filename, key_profile_name, key_transition_name):
        kp_string = key_profiles.get_as_string(key_profile_name)
        kt_string = key_transitions.get_as_string(key_transition_name)
        # self.logger.debug('kp_string:"{}", kt_string:"{}"'.format(kp_string, kt_string))
        justkeydding = subprocess.Popen(
                ('bin/justkeydding',
                '-e',
                '-K',
                '{}'.format(kp_string),
                '-T',
                '{}'.format(kt_string),
                filename),
                stdout=subprocess.PIPE)
        output, _ = justkeydding.communicate()
        try:
            score = float(output)
        except ValueError:
            self.logger.error('Failed while running justkeydding, output: {}'.format(output))
            score = 0.0
        error = 1 - score**2
        self.logger.debug('{}: {}'.format(filename, error))
        return error
