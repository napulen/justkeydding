import numpy as numpy
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import Normalizer
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import cross_val_score

from sklearn.svm import LinearSVC

X = np.load('dataset_features.pkl')
y = np.load('dataset_labels.pkl')

random_search = RandomizedSearchCV(model, param_distributions=params, cv=5, verbose=10, random_state=seed, n_iter=10)