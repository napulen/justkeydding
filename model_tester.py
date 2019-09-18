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

    dataset = dataset.Dataset('albrecht_chroma')

    clf = joblib.load(model)

    score = clf.score(dataset.X, dataset.y)
    print(score)
