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

    # Data augmentation
    # experiment1_symbolic.X, experiment1_symbolic.y = dataset.data_augmentation(
    #         experiment1_symbolic.X,
    #         experiment1_symbolic.y)

    random_seed = 1

    hidden_layer_sizes = [(24, 48), (48, 12), (24, 24)]
    activation = ['tanh', 'relu', 'logistic']
    alpha = np.abs(np.random.normal(0.0001, 0.001, 20).astype(float))
    solver = ['lbfgs', 'sgd', 'adam']
    max_iter = [100000]

    parameters = {
        'hidden_layer_sizes': hidden_layer_sizes,
        'activation': activation,
        'alpha': alpha,
        'solver': solver,
        'max_iter': max_iter
    }

    clf = RandomizedSearchCV(MLPClassifier(), param_distributions=parameters, n_iter=10, iid=False, cv=2, random_state=random_seed, verbose=10)

    clf.fit(experiment1_symbolic.X, experiment1_symbolic.y)

    print('CV RESULTS')
    print(clf.cv_results_)
    print('BEST SCORE')
    print(clf.best_score_)
    print('BEST INDEX')
    print(clf.best_index_)
    print('BEST PARAMS')
    print(clf.best_params_)