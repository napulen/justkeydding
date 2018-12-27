import subprocess
import logging
from . import key_profiles
from multiprocessing.dummy import Pool as ThreadPool

class Evaluator:
    def __init__(self, dataset):
        with open(dataset) as f:
            self.logger = logging.getLogger('evaluator')
            self.logger.info('Evaluator() <- dataset={}'.format(dataset))
            self.files = f.readlines()
            self.files = [x.strip() for x in self.files]
            self.logger.info('Evaluator() -> self.files={}'.format(self.files))

    def grade(self, population):
        ''' Grade a whole population '''
        self.logger.info('Start grade() <- population={}'.format(population))
        # with ThreadPool(len(population)) as p:
        #     grading = p.map(self.evaluate, population)
        grading = [self.evaluate(x) for x in population]
        grading = sorted(grading, key=lambda score: score[0])
        self.logger.info('Done grade() -> grading={}'.format(grading))
        return grading

    def evaluate(self, key_profile_name):
        ''' Evaluate a key profile '''
        self.logger.info('Start evaluate() <- key_profile_name={}'.format(key_profile_name))
        # error_list = [self.run_keydetection(filename, kp_string)
        #              for filename in self.files]
        with ThreadPool(max(len(self.files), 8)) as p:
            error_list = p.map(lambda f: self.run_keydetection(f, key_profile_name), self.files)
        total_error = sum(error_list)
        self.logger.info('Done evaluate() -> total_error={}'.format(total_error))
        return (total_error, key_profile_name)

    def run_keydetection(self, filename, key_profile_name):
        kp_string = key_profiles.get_as_string(key_profile_name)
        justkeydding = subprocess.Popen(
                ('bin/justkeydding',
                '-e',
                '-K',
                '{}'.format(kp_string),
                filename),
                stdout=subprocess.PIPE)
        output, _ = justkeydding.communicate()
        try:
            score = float(output)
        except ValueError:
            self.logger.error('Failed while running justkeydding, output: {}'.format(output))
            score = 0.0
        error = 1 - score**2
        self.logger.debug('{} on {}: {}'.format(key_profile_name, filename, error))
        return error
