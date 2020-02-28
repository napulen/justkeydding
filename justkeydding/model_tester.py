import justkeydding.dataset
import joblib
import os

if __name__ == '__main__':
    model = 'pre-trained.joblib'
    if not os.path.exists(model):
        print('You need to train a model first')
        exit()
    data = justkeydding.dataset.Dataset('albrecht_chroma')
    X = justkeydding.dataset.feature_scaling(data.X)
    y = data.y
    clf = joblib.load(model)
    score = clf.score(X, y)
    print(score)
