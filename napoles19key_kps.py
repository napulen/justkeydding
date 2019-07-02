import numpy as np
import dataset

names = {
    0: 'krumhansl_kessler_ktg_exponential5',
    1: 'krumhansl_kessler_ktg_exponential10',
    2: 'krumhansl_kessler_ktg_exponential15',
    3: 'aarden_essen_ktg_exponential5',
    4: 'aarden_essen_ktg_exponential10',
    5: 'aarden_essen_ktg_exponential15',
    6: 'sapp_ktg_exponential5',
    7: 'sapp_ktg_exponential10',
    8: 'sapp_ktg_exponential15',
    9: 'bellman_budge_ktg_exponential5',
    10: 'bellman_budge_ktg_exponential10',
    11: 'bellman_budge_ktg_exponential15',
    12: 'temperley_ktg_exponential5',
    13: 'temperley_ktg_exponential10',
    14: 'temperley_ktg_exponential15',
    15: 'albrecht_shanahan2_ktg_exponential5',
    16: 'albrecht_shanahan2_ktg_exponential10',
    17: 'albrecht_shanahan2_ktg_exponential15',
}

albrecht = dataset.Dataset('albrecht_symbolic')

# Removing problematic datapoints (see corrections to experiment 1)
bad_indexes = np.where(albrecht.y == -1)
albrecht.y = np.delete(albrecht.y, bad_indexes)
albrecht.X = np.delete(albrecht.X, bad_indexes, axis=0)

albrecht.X = albrecht.X.reshape(albrecht.X.shape[0], -1, 24)

predictions = np.argmax(albrecht.X, axis=2)

for idx in range(predictions.shape[0]):
    predictions[idx] = np.array([xi==albrecht.y[idx] for xi in predictions[idx]])

iterations = 10

major_scores = []
minor_scores = []
all_scores = []

for i in range(iterations):
    # sample = predictions[np.random.choice(predictions.shape[0], 492, replace=False)]
    test_indices = np.random.permutation(albrecht.y.shape[0])[:492]

    testing_X = np.take(predictions, test_indices, axis=0)
    testing_y = np.take(albrecht.y, test_indices)

    # Separating testing into major and minor keys
    testing_major_indices = np.where(testing_y < 12)[0]
    testing_minor_indices = np.where(testing_y >= 12)[0]

    testing_major_X = np.take(testing_X, testing_major_indices, axis=0)
    testing_major_y = np.take(testing_y, testing_major_indices)

    testing_minor_X = np.take(testing_X, testing_minor_indices, axis=0)
    testing_minor_y = np.take(testing_y, testing_minor_indices)

    # Evaluating
    major_correct = np.sum(testing_major_X, axis=0)
    major_score = major_correct / testing_major_X.shape[0]

    minor_correct = np.sum(testing_minor_X, axis=0)
    minor_score = minor_correct / testing_minor_X.shape[0]

    correct = np.sum(testing_X, axis=0)
    score = correct / testing_X.shape[0]

    major_scores.append(major_score)
    minor_scores.append(minor_score)
    all_scores.append(score)

major_scores = np.array(major_scores)
minor_scores = np.array(minor_scores)
all_scores = np.array(all_scores)

major_cv_mean = major_scores.mean(axis=0)
major_cv_std = major_scores.std(axis=0)

minor_cv_mean = minor_scores.mean(axis=0)
minor_cv_std = minor_scores.std(axis=0)

cv_mean = all_scores.mean(axis=0)
cv_std = all_scores.std(axis=0)

for idx in range(2, cv_mean.shape[0], 3):
    print('{:<40}: {:.1%} (std={:.1%})\t{:.1%} (std={:.1%})\t{:.1%} (std={:.1%})'.format(names[idx], major_cv_mean[idx], major_cv_std[idx], minor_cv_mean[idx], minor_cv_std[idx], cv_mean[idx], cv_std[idx]))
