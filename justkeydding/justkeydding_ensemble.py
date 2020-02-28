import justkeydding.dataset
from justkeydding.optimizer import ensembler
import numpy as np
import sys
import joblib
import os


tonic_names = [
    'C', 'C#', 'D', 'Eb',
    'E', 'F', 'F#', 'G',
    'Ab', 'A', 'Bb', 'B',
]


def key_name(key_number):
    mode = 'major' if key_number < 12 else 'minor'
    tonic = tonic_names[key_number % 12]
    return '{}\t{}'.format(tonic, mode)


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('You need to provide an input file')
        exit()
    filename = sys.argv[1]
    model = 'pre-trained.joblib'
    if not os.path.exists(model):
        print('You need to train a model first')
        exit()
    key_profiles = [
        'aarden_essen',
        'temperley',
        'krumhansl_kessler',
        'bellman_budge',
        'albrecht_shanahan1',
        'albrecht_shanahan2',
        'sapp',
        'simple_natural_minor',
        'simple_harmonic_minor',
        'simple_melodic_minor', ]
    key_transitions = [
        'ktg_exponential5',
        'ktg_exponential10',
        'ktg_exponential15', ]
    mixed_profiles = False
    ens = ensembler.Ensembler(key_profiles, key_transitions)
    features = ens.evaluate(filename, mixed_profiles)
    features = [f for l in features for f in l]
    feature_array = np.array(features).reshape(1, -1)
    feature_array = justkeydding.dataset.feature_scaling(feature_array)
    clf = joblib.load(model)
    prediction = clf.predict(feature_array)[0]
    print(key_name(prediction))
