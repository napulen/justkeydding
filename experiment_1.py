from sklearn.model_selection import cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
import numpy as np
import dataset
import joblib

if __name__ == '__main__':
    # Symbolic datasets
    napoles2018_symbolic = dataset.Dataset('napoles2018_symbolic')
    arthur2018_symbolic = dataset.Dataset('arthur2018-synth_symbolic')
    amaps_symbolic = dataset.Dataset('a-maps_symbolic')
    albrecht_symbolic = dataset.Dataset('albrecht_symbolic')

    # Audio datasets
    napoles2018_chroma = dataset.Dataset('napoles2018_chroma')
    arthur2018_chroma = dataset.Dataset('arthur2018-synth_chroma')
    amaps_chroma = dataset.Dataset('a-maps_chroma')
    albrecht_chroma = dataset.Dataset('albrecht_chroma')

    # Put them together
    # Symbolic training
    experiment1_symbolic_training_X = np.concatenate((
        napoles2018_symbolic.training_X,
        arthur2018_symbolic.training_X,
        amaps_symbolic.training_X,
        albrecht_symbolic.training_X
    ))

    experiment1_symbolic_training_y = np.concatenate((
        napoles2018_symbolic.training_y,
        arthur2018_symbolic.training_y,
        amaps_symbolic.training_y,
        albrecht_symbolic.training_y
    ))

    # Symbolic testing
    experiment1_symbolic_test_X = np.concatenate((
        napoles2018_symbolic.test_X,
        arthur2018_symbolic.test_X,
        amaps_symbolic.test_X,
        albrecht_symbolic.test_X
    ))

    experiment1_symbolic_test_y = np.concatenate((
        napoles2018_symbolic.test_y,
        arthur2018_symbolic.test_y,
        amaps_symbolic.test_y,
        albrecht_symbolic.test_y
    ))

    # Chroma training
    experiment1_chroma_training_X = np.concatenate((
        napoles2018_chroma.training_X,
        arthur2018_chroma.training_X,
        amaps_chroma.training_X,
        albrecht_chroma.training_X
    ))

    experiment1_chroma_training_y = np.concatenate((
        napoles2018_chroma.training_y,
        arthur2018_chroma.training_y,
        amaps_chroma.training_y,
        albrecht_chroma.training_y
    ))

    # Chroma testing
    experiment1_chroma_test_X = np.concatenate((
        napoles2018_chroma.test_X,
        arthur2018_chroma.test_X,
        amaps_chroma.test_X,
        albrecht_chroma.test_X
    ))

    experiment1_chroma_test_y = np.concatenate((
        napoles2018_chroma.test_y,
        arthur2018_chroma.test_y,
        amaps_chroma.test_y,
        albrecht_chroma.test_y
    ))

    # Hybrid training
    experiment1_hybrid_training_X = np.concatenate((
        experiment1_symbolic_training_X,
        experiment1_chroma_training_X
    ))

    experiment1_hybrid_training_y = np.concatenate((
        experiment1_symbolic_training_y,
        experiment1_chroma_training_y
    ))

    # Feature scaling in all the feature vectors
    experiment1_symbolic_training_X = dataset.feature_scaling(experiment1_symbolic_training_X)
    experiment1_symbolic_test_X = dataset.feature_scaling(experiment1_symbolic_test_X)
    experiment1_chroma_training_X = dataset.feature_scaling(experiment1_chroma_training_X)
    experiment1_chroma_test_X = dataset.feature_scaling(experiment1_chroma_test_X)
    experiment1_hybrid_training_X = dataset.feature_scaling(experiment1_hybrid_training_X)

    # Data augmentation on the training sets
    experiment1_symbolic_training_X, experiment1_symbolic_training_y = dataset.data_augmentation(
        experiment1_symbolic_training_X,
        experiment1_symbolic_training_y)

    experiment1_chroma_training_X, experiment1_chroma_training_y = dataset.data_augmentation(
        experiment1_chroma_training_X,
        experiment1_chroma_training_y)

    experiment1_hybrid_training_X, experiment1_hybrid_training_y = dataset.data_augmentation(
        experiment1_hybrid_training_X,
        experiment1_hybrid_training_y
    )

    # Train the classifiers
    clf_symbolic = SVC(kernel='linear', max_iter=100000)
    clf_chroma = SVC(kernel='linear', max_iter=100000)
    clf_hybrid = SVC(kernel='linear', max_iter=100000)

    # clf_symbolic = LogisticRegression(penalty='l1', max_iter=100000)
    # clf_chroma = LogisticRegression(penalty='l1', max_iter=100000)
    # clf_hybrid = LogisticRegression(penalty='l1', max_iter=100000)


    clf_symbolic.fit(experiment1_symbolic_training_X, experiment1_symbolic_training_y)
    clf_name = 'clf_symbolic.joblib'
    joblib.dump(clf_symbolic, clf_name)

    clf_chroma.fit(experiment1_chroma_training_X, experiment1_chroma_training_y)
    clf_name = 'clf_chroma.joblib'
    joblib.dump(clf_chroma, clf_name)

    clf_hybrid.fit(experiment1_hybrid_training_X, experiment1_hybrid_training_y)
    clf_name = 'clf_hybrid.joblib'
    joblib.dump(clf_hybrid, clf_name)

    # Get the predictions
    # Symbolic on symbolic
    pred_symbolic_symbolic = clf_symbolic.predict(
        experiment1_symbolic_test_X
    )

    score_symbolic_symbolic = clf_symbolic.score(
        experiment1_symbolic_test_X,
        experiment1_symbolic_test_y
    )

    all_scores_symbolic_symbolic = dataset.weighted_score(
        pred_symbolic_symbolic,
        experiment1_symbolic_test_y
    )

    print('Symbolic on symbolic')
    print(pred_symbolic_symbolic)
    print(score_symbolic_symbolic)
    print(all_scores_symbolic_symbolic)

    # Symbolic on chroma
    pred_symbolic_chroma = clf_symbolic.predict(
        experiment1_chroma_test_X
    )

    score_symbolic_chroma = clf_symbolic.score(
        experiment1_chroma_test_X,
        experiment1_chroma_test_y
    )

    all_scores_symbolic_chroma = dataset.weighted_score(
        pred_symbolic_chroma,
        experiment1_chroma_test_y
    )

    print('Symbolic on chroma')
    print(pred_symbolic_chroma)
    print(score_symbolic_chroma)
    print(all_scores_symbolic_chroma)

    # Chroma on symbolic
    pred_chroma_symbolic = clf_chroma.predict(
        experiment1_symbolic_test_X
    )

    score_chroma_symbolic = clf_chroma.score(
        experiment1_symbolic_test_X,
        experiment1_symbolic_test_y
    )

    all_scores_chroma_symbolic = dataset.weighted_score(
        pred_chroma_symbolic,
        experiment1_symbolic_test_y
    )

    print('Chroma on symbolic')
    print(pred_chroma_symbolic)
    print(score_chroma_symbolic)
    print(all_scores_chroma_symbolic)

    # Chroma on chroma
    pred_chroma_chroma = clf_chroma.predict(
        experiment1_chroma_test_X
    )

    score_chroma_chroma = clf_chroma.score(
        experiment1_chroma_test_X,
        experiment1_chroma_test_y
    )

    all_scores_chroma_chroma = dataset.weighted_score(
        pred_chroma_chroma,
        experiment1_chroma_test_y
    )

    print('Chroma on chroma')
    print(pred_chroma_chroma)
    print(score_chroma_chroma)
    print(all_scores_chroma_chroma)

    # Hybrid on symbolic
    pred_hybrid_symbolic = clf_hybrid.predict(
        experiment1_symbolic_test_X
    )

    score_hybrid_symbolic = clf_hybrid.score(
        experiment1_symbolic_test_X,
        experiment1_symbolic_test_y
    )

    all_scores_hybrid_symbolic = dataset.weighted_score(
        pred_hybrid_symbolic,
        experiment1_symbolic_test_y
    )

    print('Hybrid on symbolic')
    print(pred_hybrid_symbolic)
    print(score_hybrid_symbolic)
    print(all_scores_hybrid_symbolic)

    # Hybrid on chroma
    pred_hybrid_chroma = clf_hybrid.predict(
        experiment1_chroma_test_X
    )

    score_hybrid_chroma = clf_hybrid.score(
        experiment1_chroma_test_X,
        experiment1_chroma_test_y
    )

    all_scores_hybrid_chroma = dataset.weighted_score(
        pred_hybrid_chroma,
        experiment1_chroma_test_y
    )

    print('Hybrid on chroma')
    print(pred_hybrid_chroma)
    print(score_hybrid_chroma)
    print(all_scores_hybrid_chroma)