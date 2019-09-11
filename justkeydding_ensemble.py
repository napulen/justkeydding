import numpy as np
import dataset
import sys
import joblib
from optimizer import ensembler

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('You need to provide a file and a model')
        exit()
    filename = sys.argv[1]
    model = sys.argv[2]
    # TODO: Get the key profiles and transitions from the model
    # not the datasets
    ensemble = dataset.Dataset('napoles2018_symbolic').ensemble
    print(ensemble)
    ###########################################################
    ens = ensembler.Ensembler(ensemble['key_profiles'], ensemble['key_transitions'])
    features = ens.evaluate(filename, ensemble['mixed_profiles'])
    features = [f for l in features for f in l]
    feature_array = np.array(features).reshape(1, -1)
    clf = joblib.load(model)
    prediction = clf.predict(feature_array)
    print(prediction)
