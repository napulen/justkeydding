from sklearn.model_selection import ShuffleSplit
from sklearn.model_selection import RandomizedSearchCV
from sklearn.linear_model import LogisticRegression
from sklearn.neural_network import MLPClassifier
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

    penalty = ['l2']
    dual = [False]
    fit_intercept = [True, False]
    intercept_scaling = np.abs(np.random.normal(1, 0.2, 5).astype(float))
    solver = ['saga']
    multi_class = ['auto']
    max_iter = [100000]
    C = np.abs(np.random.normal(1, 0.2, 20).astype(float))

    parameters = { 
        'C': C,
        'dual': dual,
        'fit_intercept': fit_intercept,
        'intercept_scaling': intercept_scaling,
        'solver': solver,
        'multi_class': multi_class,
        'penalty': penalty,
        'max_iter': max_iter
    }

    clf = RandomizedSearchCV(LogisticRegression(), param_distributions=parameters, n_iter=50, iid=False, cv=2, random_state=random_seed, verbose=2)

    clf.fit(experiment1_symbolic.X, experiment1_symbolic.y)

    print('CV RESULTS')
    print(clf.cv_results_)
    print('BEST SCORE')
    print(clf.best_score_)
    print('BEST INDEX')
    print(clf.best_index_)
    print('BEST PARAMS')
    print(clf.best_params_)