import numpy as np
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import MinMaxScaler
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.svm import LinearSVC
from sklearn.utils import shuffle

def transpose(Xi, y):
    is_minor = y // 12
    trans_features = []
    trans_labels = []
    for ti in range(12):
        Xip = Xi.reshape(-1, 12)
        label = ((y + ti) % 12) + (12 * is_minor)
        features = np.roll(Xip, ti, axis=1)
        trans_features.append(features)
        trans_labels.append(label)
    trans_features = np.array(trans_features).reshape(12, -1)
    trans_labels = np.array(trans_labels)
    return np.array(trans_features), np.array(trans_labels)

def data_augmentation(X, y):
    original_length = X.shape[1]
    trans_features = []
    trans_labels = []
    for i in range(len(X)):
        Xp, yp = transpose(X[i], y[i])
        trans_features.append(Xp)
        trans_labels.append(yp)
    trans_features = np.array(trans_features).reshape(-1, original_length)
    trans_labels = np.array(trans_labels).reshape(-1)
    return trans_features, trans_labels

def feature_scaling(X):
    scaler = MinMaxScaler()
    original_length = X.shape[1]
    Xp = X.reshape(-1, 24)
    Xp = np.apply_along_axis(lambda x: scaler.fit_transform(x.reshape(-1, 1)), 1, Xp)
    Xp = Xp.reshape(-1, original_length)
    return Xp

if __name__ == '__main__':
    X = np.load('dataset_features.pkl')
    y = np.load('dataset_labels.pkl')
    X = feature_scaling(X)
    Xa, ya = data_augmentation(X, y)
    Xa, ya = shuffle(Xa, ya)
    clf = LinearSVC(max_iter=10000)
    cross_val_score(clf, Xa, ya, cv=10)
