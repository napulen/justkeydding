import subprocess
import logging
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

    def evaluate(self, key_profile):
        ''' Evaluate a key profile '''
        self.logger.info('Start evaluate() <- key_profile={}'.format(key_profile))
        kp_string = ['{:.12f}'.format(p) for p in key_profile]
        kp_string = ' '.join(kp_string)
        # error_list = [self.run_keydetection(filename, kp_string)
        #              for filename in self.files]
        with ThreadPool(max(len(self.files), 8)) as p:
            error_list = p.map(lambda f: self.run_keydetection(f, kp_string), self.files)
        total_error = sum(error_list)
        self.logger.info('Done evaluate() -> total_error={}'.format(total_error))
        return (total_error, key_profile)

    def run_keydetection(self, filename, kp_string):
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
        error = 1 - score
        self.logger.debug('{}: {}'.format(filename, error))
        return error
