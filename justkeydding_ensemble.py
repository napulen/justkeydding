import numpy as np
import dataset
import sys
import joblib
import os
from optimizer import ensembler

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
    # TODO: Get the key profiles and transitions from the model
    # not the datasets
    ensemble = dataset.Dataset('napoles2018_symbolic').ensemble
    # print(ensemble)
    ###########################################################
    ens = ensembler.Ensembler(ensemble['key_profiles'], ensemble['key_transitions'])
    features = ens.evaluate(filename, ensemble['mixed_profiles'])
    features = [f for l in features for f in l]
    feature_array = np.array(features).reshape(1, -1)
    clf = joblib.load(model)
    prediction = clf.predict(feature_array)[0]
    print(key_name(prediction))
