import numpy as np
from sklearn.preprocessing import MinMaxScaler
import collections
import re
import os


class Dataset():
    def __init__(self, dataset_name):
        self.dataset_dir = 'datasets'
        self.dataset_name = dataset_name
        if not os.path.exists(self.dataset_dir):
            print('Could not find the dataset dir')
            exit()
        files = []
        for f in os.listdir(self.dataset_dir):
            if f.startswith(self.dataset_name):
                files.append(f)
        annotations_re = re.escape(self.dataset_name) + '-?(.*)_annotations.pkl'
        features_re = re.escape(self.dataset_name) + '-?(.*)_features.pkl'
        ensemble_file = '{}_ensemble.txt'.format(self.dataset_name)
        for f in files:
            # print('Loading {}...'.format(f))
            if f == ensemble_file:
                with open(os.path.join(self.dataset_dir, f)) as fd:
                    lines = fd.readlines()
                if len(lines) != 3:
                    print('The format of the ensemble metadata is incorrect.')
                    exit()
                kps, kts, mixed_profiles = lines
                kps = kps.split(',')
                kps = [s.strip() for s in kps]
                kts = kts.split(',')
                kts = [s.strip() for s in kts]
                mixed_profiles = mixed_profiles.split('=')[1] == "True"
                ensemble = {
                    'key_profiles': kps,
                    'key_transitions': kts,
                    'mixed_profiles': mixed_profiles
                }
                setattr(self, 'ensemble', ensemble)
            match = re.search(features_re, f, re.IGNORECASE)
            if match:
                split_name = match.group(1)
                array = np.load(
                    os.path.join(self.dataset_dir, f),
                    allow_pickle=True
                )
                if split_name:
                    attr_name = '{}_X'.format(split_name)
                    setattr(self, attr_name, array)
                else:
                    self.X = array
            else:
                match = re.search(annotations_re, f, re.IGNORECASE)
                if match:
                    split_name = match.group(1)
                    array = np.load(
                        os.path.join(self.dataset_dir, f),
                        allow_pickle=True
                    )
                    if split_name:
                        attr_name = '{}_y'.format(split_name)
                        setattr(self, attr_name, array)
                    else:
                        self.y = array


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


def compare_keys(k1, k2):
    if k2 == k1:
        return 'tonic'
    if k1 < 12:
        dominant = (k1 + 7) % 12
        subdominant = (k1 + 5) % 12
        relative = ((k1 + 9) % 12) + 12
        parallel = k1 + 12
    else:
        dominant = ((k1 + 7) % 12) + 12
        subdominant = ((k1 + 5) % 12) + 12
        relative = (k1 - 9) % 12
        parallel = k1 - 12
    if k2 == dominant:
        return 'fifth'
    elif k2 == subdominant:
        return 'fifth'
    elif k2 == relative:
        return 'relative'
    elif k2 == parallel:
        return 'parallel'
    else:
        return 'other'


def weighted_score(pred, y):
    length = len(pred)
    if length != len(y):
        print('These arrays should have the same number of elements')
        return
    keys = [compare_keys(y[i], pred[i]) for i in range(length)]
    cnt = collections.Counter(keys)
    tonics = cnt.get('tonic', 0)
    fifths = cnt.get('fifth', 0)
    relatives = cnt.get('relative', 0)
    parallels = cnt.get('parallel', 0)
    others = cnt.get('other', 0)
    mirex_score = tonics + 0.5*fifths + 0.3*relatives + 0.2*parallels
    all_scores = [mirex_score, tonics, fifths, relatives, parallels, others]
    all_scores = [x / length for x in all_scores]
    return all_scores


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


def feature_scaling(X, minmax=(-1, 1)):
    scaler = MinMaxScaler(feature_range=minmax)
    original_length = X.shape[1]
    Xp = X.reshape(-1, 24)
    Xp = np.apply_along_axis(
        lambda x: scaler.fit_transform(x.reshape(-1, 1)), 1, Xp
    )
    Xp = Xp.reshape(-1, original_length)
    return Xp
