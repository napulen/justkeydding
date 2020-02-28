from justkeydding.parameters import key_profiles, key_transitions
import random
import logging


class Generator:
    def __init__(self, kp_max_range, kt_max_range):
        self.logger = logging.getLogger('generator')
        self.kp_maximum_range = kp_max_range
        self.kt_maximum_range = kt_max_range
        self.kp_id = 0
        self.kt_id = 0
        self.logger.info("Generator() <- kp_max_range={}, kt_max_range={}".format(self.kp_maximum_range, self.kt_maximum_range))

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
                r = random.randrange(int(self.kp_maximum_range / 2**hierarchy))
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
                r = random.randrange(int(self.kp_maximum_range / 2**hierarchy))
                key_profile[idx] = r
                self.logger.debug('{}: {}'.format(idx, r))
        s = sum(key_profile)
        key_profile = [x/s for x in key_profile]
        self.logger.info('Done generate_minor_key_profile() -> key_profile={}'.format(key_profile))
        return key_profile

    def generate_key_transition(self):
        ''' Generate a set of random key transitions '''
        self.logger.info("Start generate_key_transition()")
        kt_type = random.choice(['geometric', 'geometric', 'geometric', 'geometric', 'geometric', 'geometric', 'geometric', 'geometric', 'geometric', 'stochastic'])
        if kt_type == 'geometric':
            ratio = random.random() * 48.0 + 2.0
            key_transition = self.generate_geometric_key_transition(ratio)
            key_transition_name = 'ktg{}'.format(self.kt_id)
            key_transitions.store_ratio(key_transition_name, ratio)
        elif kt_type == 'stochastic':
            key_transition = self.generate_stochastic_key_transition()
            key_transition_name = 'kts{}'.format(self.kt_id)
        self.kt_id += 1
        key_transitions.insert_new(key_transition_name, key_transition)
        self.logger.info('Done generate_key_transition() -> key_transition_name={}'.format(key_transition_name))
        return key_transition_name

    def generate_geometric_key_transition(self, ratio):
        self.logger.info('Start generate_geometric_key_transition() <- ratio={}'.format(ratio))
        key_transition = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ]
        groups = [
            [0],
            [5, 7, 12, 21],
            [14, 16, 17, 19],
            [2, 3, 9, 10],
            [4, 8, 22, 23],
            [1, 11],
            [15, 18],
            [13, 20],
            [6]
        ]
        number_of_groups = len(groups) - 1
        for group_id, keys in enumerate(groups):
            for key in keys:
                r = ratio**(number_of_groups - group_id)
                key_transition[key] = r
                self.logger.debug('{}: {}'.format(key, r))
        s = sum(key_transition)
        key_transition = [x/s for x in key_transition]
        self.logger.info('Done generate_geometric_key_transition() -> key_transition={}'.format(key_transition))
        return key_transition

    def generate_stochastic_key_transition(self):
        self.logger.info('Start generate_stochastic_key_transition()')
        key_transition = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ]
        groups = [
            [0],
            [5, 7, 12, 21],
            [14, 16, 17, 19],
            [2, 3, 9, 10],
            [4, 8, 22, 23],
            [1, 11],
            [15, 18],
            [13, 20],
            [6]
        ]
        for group_id, keys in enumerate(groups):
            for key in keys:
                r = random.randrange(int(self.kt_maximum_range / 2**group_id))
                key_transition[key] = r
                self.logger.debug('{}: {}'.format(key, r))
        s = sum(key_transition)
        key_transition = [x/s for x in key_transition]
        self.logger.info('Done generate_stochastic_key_transition() -> key_transition={}'.format(key_transition))
        return key_transition

    def generate_key_profiles(self, count):
        ''' Generate <count> key_profiles '''
        self.logger.info('Start generate_key_profiles() <- count={}'.format(count))
        key_profiles = [self.generate_key_profile() for _ in range(count)]
        self.logger.info('Done generate_key_profiles() -> key_profiles={}'.format(key_profiles))
        return key_profiles

    def generate_key_transitions(self, count):
        ''' Generate <count> key_transitions '''
        self.logger.info('Start generate_key_transitions() <- count={}'.format(count))
        key_transitions = [self.generate_key_transition() for _ in range(count)]
        self.logger.info('Done generate_key_transitions() -> key_transitions={}'.format(key_transitions))
        return key_transitions
