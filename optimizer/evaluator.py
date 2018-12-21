import subprocess
import logging

class Evaluator:
    def __init__(self, dataset):
        with open(dataset) as f:
            self.files = f.readlines()
            self.files = [x.strip() for x in self.files]

    def grade(self, population):
        ''' Grade a whole population '''
        grading = [(self.evaluate(x), x) for x in population]
        grading = sorted(grading, key=lambda score: score[0])
        return grading

    def evaluate(self, key_profile):
        ''' Evaluate a key profile '''
        kp_string = ''
        for p in key_profile:
            kp_string = kp_string + '{:.12f} '.format(p)
        kp_string = kp_string[:-1]
        total_error = 0
        for filename in self.files:
            print(filename)
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
                score = 0.0
            error = 1 - score
            total_error += error
        return total_error
