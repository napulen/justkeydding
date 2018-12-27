import random
import logging
from . import key_profiles

class Generator:
    def __init__(self, maximum_range):
        self.logger = logging.getLogger('generator')
        self.maximum_range = maximum_range
        self.kp_id = 0
        self.logger.info("Generator() <- maximum_range={}".format(self.maximum_range))

    def generate_key_profile(self):
        ''' Generate a set of random key profiles '''
        self.logger.info('Start generate_key_profile()')
        major = self.generate_major_key_profile()
        minor = self.generate_minor_key_profile()
        key_profile = major + minor
        key_profile_name = 'kp{}'.format(self.kp_id)
        self.kp_id += 1
        key_profiles.insert_new(key_profile_name, key_profile)
        self.logger.info('Done generate_key_profile() -> key_profile_name={}'.format(key_profile_name))
        return key_profile_name

    def generate_major_key_profile(self):
        self.logger.info('Start generate_major_key_profile()')
        key_profile = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ]
        tonic_dominant = [0, 7]
        diatonic = [2, 4, 5, 9, 11]
        non_diatonic = [1, 3, 6, 8, 10]
        groups = [
            (tonic_dominant, 0),
            (diatonic, 1),
            (non_diatonic, 2)
        ]
        for group, hierarchy in groups:
            for idx in group:
                r = random.randrange(int(self.maximum_range / 2**hierarchy))
                key_profile[idx] = r
                self.logger.debug('{}: {}'.format(idx, r))
        s = sum(key_profile)
        key_profile = [x/s for x in key_profile]
        self.logger.info('Done generate_major_key_profile() -> key_profile={}'.format(key_profile))
        return key_profile

    def generate_minor_key_profile(self):
        self.logger.info('Start generate_minor_key_profile()')
        key_profile = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ]
        tonic_dominant = [0, 7]
        harmonic = [2, 3, 5, 8, 11]
        natural_melodic = [9, 10]
        non_diatonic = [1, 4, 6]
        groups = [
            (tonic_dominant, 0),
            (harmonic, 1),
            (natural_melodic, 2),
            (non_diatonic, 3)
        ]
        for group, hierarchy in groups:
            for idx in group:
                r = random.randrange(int(self.maximum_range / 2**hierarchy))
                key_profile[idx] = r
                self.logger.debug('{}: {}'.format(idx, r))
        s = sum(key_profile)
        key_profile = [x/s for x in key_profile]
        self.logger.info('Done generate_minor_key_profile() -> key_profile={}'.format(key_profile))
        return key_profile

    def generate_population(self, count):
        ''' Generate <count> key_profiles for a population '''
        self.logger.info('Start generate_population() <- count={}'.format(count))
        population = [self.generate_key_profile() for _ in range(count)]
        self.logger.info('Done generate_population() -> population={}'.format(population))
        return population
