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
albrecht.X = albrecht.X.reshape(981, -1, 24)

predictions = np.argmax(albrecht.X, axis=2)

for idx in range(len(predictions)):
    predictions[idx] = np.array([xi==albrecht.y[idx] for xi in predictions[idx]])


iterations = 10

all_scores = []

for i in range(iterations):

    sample = predictions[np.random.choice(predictions.shape[0], 492, replace=False)]

    corrects = np.sum(sample, axis=0)

    scores = corrects / sample.shape[0]

    all_scores.append(scores)

all_scores = np.array(all_scores)

cv_mean = all_scores.mean(axis=0)
cv_std = all_scores.std(axis=0)

for idx in range(2, cv_mean.shape[0], 3):
    print('{}: {} (std={})'.format(names[idx], cv_mean[idx], cv_std[idx]))
