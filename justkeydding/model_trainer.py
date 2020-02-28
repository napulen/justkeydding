import justkeydding.dataset
from sklearn.linear_model import LogisticRegression
import numpy as np
import joblib

if __name__ == '__main__':

    clf_name = 'pre-trained'

    dataset_filenames = [
        'a-maps_chroma',
        # 'a-maps_symbolic',
        'albrecht_chroma',
        # 'albrecht_symbolic',
        'arthur2018-recordings_chroma',
        'arthur2018-synth-shortname_chroma',
        # 'arthur2018-synth-shortname_symbolic',
        'billboard_chroma',
        'cross-era-shortname_chroma',
        'giantsteps-key_chroma',
        'giantsteps-mtg-key_chroma',
        'napoles2018_chroma',
        # 'napoles2018_symbolic',
    ]

    for idx, d in enumerate(dataset_filenames):
        data = justkeydding.dataset.Dataset(d)
        if idx == 0:
            ensemble = data.ensemble
            fulldataset_X = data.X
            fulldataset_y = data.y
        else:
            if data.ensemble != ensemble:
                print('This dataset is incompatible with other datasets')
                exit()
            fulldataset_X = np.concatenate((fulldataset_X, data.X))
            fulldataset_y = np.concatenate((fulldataset_y, data.y))

    clf = LogisticRegression(
        penalty='l2',
        fit_intercept=False,
        dual=False,
        C=0.7,
        solver='lbfgs',
        multi_class='auto',
        max_iter=100000
    )

    # Feature scaling in all the feature vectors
    fulldataset_X = justkeydding.dataset.feature_scaling(fulldataset_X)

    # Data augmentation on the training sets
    fulldataset_X, fulldataset_y = justkeydding.dataset.data_augmentation(
        fulldataset_X,
        fulldataset_y)

    # Training
    clf.fit(fulldataset_X, fulldataset_y)

    # Save the model
    clf_filename = '{}.joblib'.format(clf_name)
    joblib.dump(clf, clf_filename)
