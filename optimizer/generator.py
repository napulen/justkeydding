import random
import logging

class Generator:
    def __init__(self, maximum_range):
        self.maximum_range = maximum_range

    def generate_key_profile(self):
        ''' Generate a set of random key profiles '''
        major = self.generate_major_key_profile()
        minor = self.generate_minor_key_profile()
        return major + minor

    def generate_major_key_profile(self):
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
        s = sum(key_profile)
        return [x/s for x in key_profile]

    def generate_minor_key_profile(self):
        key_profile = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        ]
        tonic_dominant = [0, 7]
        harmonic = [2, 3, 5, 8, 11]
        natural_melodic = [4, 9, 10]
        non_diatonic = [1, 6]
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
        s = sum(key_profile)
        return [x/s for x in key_profile]

    def generate_population(self, count):
        ''' Generate <count> key_profiles for a population '''
        return [self.generate_key_profile() for _ in range(count)]