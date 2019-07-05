from sklearn.model_selection import GridSearchCV
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
    
    # # Data augmentation on the training sets
    experiment1_symbolic.X, experiment1_symbolic.y = dataset.data_augmentation(
        experiment1_symbolic.X,
        experiment1_symbolic.y)


    clf_symbolic = LogisticRegression(penalty='l2', max_iter=100000)

    scores = cross_val_score(clf_symbolic, experiment1_symbolic.X, experiment1_symbolic.y, cv=2)

    print(scores)