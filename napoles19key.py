from sklearn.model_selection import ShuffleSplit
from sklearn.model_selection import cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
import numpy as np
import dataset
import joblib

if __name__ == '__main__':
    # Symbolic datasets
    experiment1_symbolic = dataset.Dataset('albrecht_symbolic')

    # Removing problematic datapoints (see corrections to experiment 1)
    bad_indexes = np.where(experiment1_symbolic.y == -1)
    experiment1_symbolic.y = np.delete(experiment1_symbolic.y, bad_indexes)
    experiment1_symbolic.X = np.delete(experiment1_symbolic.X, bad_indexes, axis=0)

    # Feature scaling in all the feature vectors
    experiment1_symbolic.X = dataset.feature_scaling(experiment1_symbolic.X)

    random_seed = 1
    splits = 20

    clf_symbolic = LogisticRegression(penalty='l2', solver='lbfgs', multi_class='auto', max_iter=100000)

    rs = ShuffleSplit(n_splits=splits, random_state=random_seed, test_size=492, train_size=None)

    major_scores = []
    minor_scores = []
    scores = []

    for training_indices, testing_indices in rs.split(experiment1_symbolic.X, experiment1_symbolic.y):
        # print(training_indices, testing_indices)
        training_X = np.take(experiment1_symbolic.X, training_indices, axis=0)
        training_y = np.take(experiment1_symbolic.y, training_indices)

        testing_X = np.take(experiment1_symbolic.X, testing_indices, axis=0)
        testing_y = np.take(experiment1_symbolic.y, testing_indices)

        # Data augmentation on the training sets
        training_X, training_y = dataset.data_augmentation(
            training_X,
            training_y)

        # Separating testing into major and minor keys
        testing_major_indices = np.where(testing_y < 12)[0]
        testing_minor_indices = np.where(testing_y >= 12)[0]

        testing_major_X = np.take(testing_X, testing_major_indices, axis=0)
        testing_major_y = np.take(testing_y, testing_major_indices)

        testing_minor_X = np.take(testing_X, testing_minor_indices, axis=0)
        testing_minor_y = np.take(testing_y, testing_minor_indices)

        # Training
        clf_symbolic.fit(training_X, training_y)
        
        # Testing
        major_score = clf_symbolic.score(testing_major_X, testing_major_y)
        minor_score = clf_symbolic.score(testing_minor_X, testing_minor_y)
        score = clf_symbolic.score(testing_X, testing_y)

        print('{:.1%}\t{:.1%}\t{:.1%}'.format(major_score, minor_score, score))
        major_scores.append(major_score)
        minor_scores.append(minor_score)
        scores.append(score)

    scores = np.array(scores)
    major_scores = np.array(major_scores)
    minor_scores = np.array(minor_scores)
    print('\nAverages: {:.1%} (std={:.1%})\t{:.1%} (std={:.1%})\t{:.1%} (std={:.1%})'.format(major_scores.mean(), major_scores.std(), minor_scores.mean(), minor_scores.std(), scores.mean(), scores.std()))


    # scores = cross_val_score(clf_symbolic, experiment1_symbolic.X, experiment1_symbolic.y, cv=2)