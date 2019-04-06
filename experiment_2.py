from sklearn.model_selection import cross_val_predict
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.pipeline import Pipeline
from sklearn.neural_network import MLPClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import GaussianNB
import numpy as np
import dataset
import joblib

if __name__ == '__main__':
    # Source datasets
    experiment1_symbolic = dataset.Dataset('experiment1_symbolic')
    experiment1_chroma = dataset.Dataset('experiment1_chroma')
    crossera_chroma = dataset.Dataset('cross-era_chroma')
    giantstepsmtg_chroma = dataset.Dataset('giantsteps-mtg-key_chroma')
    giantsteps_chroma =dataset.Dataset('giantsteps-key_chroma')
    billboard_chroma = dataset.Dataset('billboard_chroma')

    # Training set
    experiment2_training_X = np.concatenate((
        experiment1_symbolic.X,
        experiment1_chroma.X,
        #crossera_chroma.X,
        #giantstepsmtg_chroma.X,
        #billboard_chroma.training_X,
        #billboard_chroma.validation_X,
    ))

    experiment2_training_y = np.concatenate((
        experiment1_symbolic.y,
        experiment1_chroma.y,
        #crossera_chroma.y,
        #giantstepsmtg_chroma.y,
        #billboard_chroma.training_y,
        #billboard_chroma.validation_y
    ))

    # Removing problematic datapoints (see corrections to experiment 1)
    bad_indexes = np.where(experiment2_training_y == -1)
    experiment2_training_y = np.delete(experiment2_training_y, bad_indexes)
    experiment2_training_X = np.delete(experiment2_training_X, bad_indexes, axis=0)

    bad_indexes = np.where(billboard_chroma.validation_y == -1)
    billboard_chroma.validation_y = np.delete(billboard_chroma.validation_y, bad_indexes)
    billboard_chroma.validation_X = np.delete(billboard_chroma.validation_X, bad_indexes, axis=0)

    bad_indexes = np.where(billboard_chroma.test_y == -1)
    billboard_chroma.test_y = np.delete(billboard_chroma.test_y, bad_indexes)
    billboard_chroma.test_X = np.delete(billboard_chroma.test_X, bad_indexes, axis=0)

    bad_indexes = np.where(giantstepsmtg_chroma.validation_y == -1)
    giantstepsmtg_chroma.validation_y = np.delete(giantstepsmtg_chroma.validation_y, bad_indexes)
    giantstepsmtg_chroma.validation_X = np.delete(giantstepsmtg_chroma.validation_X, bad_indexes, axis=0)

    bad_indexes = np.where(giantsteps_chroma.y == -1)
    giantsteps_chroma.y = np.delete(giantsteps_chroma.y, bad_indexes)
    giantsteps_chroma.X = np.delete(giantsteps_chroma.X, bad_indexes, axis=0)

    # Feature scaling in all the feature vectors
    # Training set
    experiment2_training_X = dataset.feature_scaling(experiment2_training_X)
    # Validation sets
    billboard_chroma.validation_X = dataset.feature_scaling(billboard_chroma.validation_X)
    giantstepsmtg_chroma.validation_X = dataset.feature_scaling(giantstepsmtg_chroma.validation_X)
    # Test sets
    billboard_chroma.test_X = dataset.feature_scaling(billboard_chroma.test_X)
    giantsteps_chroma.X = dataset.feature_scaling(giantsteps_chroma.X)

    # Data augmentation on the training set
    experiment2_training_X, experiment2_training_y = dataset.data_augmentation(
        experiment2_training_X,
        experiment2_training_y
    )

    # Classifier
    clf = LogisticRegression(penalty='l2', max_iter=100000)
    clf.fit(experiment2_training_X, experiment2_training_y)

    # Get the predictions
    # Billboard validation set
    billboard_predictions = clf.predict(
        billboard_chroma.test_X
    )

    billboard_all_scores = dataset.weighted_score(
        billboard_predictions,
        billboard_chroma.test_y
    )

    print('Score on Billboard validation set')
    print(billboard_predictions)
    print(billboard_all_scores)

    # Giantsteps validation set
    giantsteps_predictions = clf.predict(
        giantsteps_chroma.X
    )

    giantsteps_all_scores = dataset.weighted_score(
        giantsteps_predictions,
        giantsteps_chroma.y
    )

    print('Score on Giantsteps validation set')
    print(giantsteps_predictions)
    print(giantsteps_all_scores)