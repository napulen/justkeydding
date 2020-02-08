import numpy as np
import dataset
import sys
import joblib
import os
from optimizer import ensembler

if __name__ == '__main__':
    
    model = 'pre-trained.joblib'
    if not os.path.exists(model):
        print('You need to train a model first')
        exit()

    data = dataset.Dataset('albrecht_chroma')

    X = dataset.feature_scaling(data.X)
    y = data.y

    clf = joblib.load(model)

    score = clf.score(X, y)
    print(score)
