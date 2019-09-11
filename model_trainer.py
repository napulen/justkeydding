from sklearn.model_selection import cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
import numpy as np
import dataset
import joblib

if __name__ == '__main__':

    clf_name = 'testmodel'

    dataset_filenames = [
        'napoles2018_symbolic',
        'arthur2018-synth_symbolic',
        'albrecht_chroma',
    ]

    for idx, d in enumerate(dataset_filenames):
        data = dataset.Dataset(d)
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
    fulldataset_X = dataset.feature_scaling(fulldataset_X)

    # Data augmentation on the training sets
    fulldataset_X, fulldataset_y = dataset.data_augmentation(
        fulldataset_X,
        fulldataset_y)

    # Training
    clf.fit(fulldataset_X, fulldataset_y)

    # Save the model
    clf_filename = '{}.joblib'.format(clf_name)
    joblib.dump(clf, clf_filename)