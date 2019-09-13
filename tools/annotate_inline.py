import os
import sys
from shutil import copyfile

keys_single_letter = {
    'C': 0, 'C#': 1, 'Db': 1, 'D': 2,
    'D#': 3, 'Eb': 3, 'E': 4, 'F': 5,
    'F#': 6, 'Gb': 6, 'G': 7, 'G#': 8,
    'Ab': 8, 'A': 9, 'A#': 10, 'Bb': 10,
    'B': 11, 'H': 11,

    'c': 12, 'c#': 13, 'db': 13, 'd': 14,
    'd#': 15, 'eb': 15, 'e': 16, 'f': 17,
    'f#': 18, 'gb': 18, 'g': 19, 'g#': 20,
    'ab': 20, 'a': 21, 'a#': 22, 'bb': 22,
    'b': 23, 'h': 23
}

keys_single_reverse = {
    -1: 'Unknown',
    0: 'C', 1: 'Db', 2: 'D', 3: 'Eb', 
    4: 'E', 5: 'F', 6: 'F#', 7: 'G', 
    8: 'Ab', 9: 'A', 10: 'Bb', 11: 'B',

    12: 'c', 13: 'c#', 14: 'd', 15: 'eb', 
    16: 'e', 17: 'f', 18: 'f#', 19: 'g', 
    20: 'ab', 21: 'a', 22: 'bb', 23: 'b'
}

keys_duple = {
    ('c', 'major'): 0, ('c#', 'major'): 1,
    ('db', 'major'): 1, ('d', 'major'): 2,
    ('d#', 'major'): 3, ('eb', 'major'): 3,
    ('e', 'major'): 4, ('f', 'major'): 5,
    ('f#', 'major'): 6, ('gb', 'major'): 6,
    ('g', 'major'): 7, ('g#', 'major'): 8,
    ('ab', 'major'): 8, ('a', 'major'): 9,
    ('a#', 'major'): 10, ('bb', 'major'): 10,
    ('b', 'major'): 11, ('h', 'major'): 11,

    ('c', 'minor'): 12, ('c#', 'minor'): 13,
    ('db', 'minor'): 13, ('d', 'minor'): 14,
    ('d#', 'minor'): 15, ('eb', 'minor'): 15,
    ('e', 'minor'): 16, ('f', 'minor'): 17,
    ('f#', 'minor'): 18, ('gb', 'minor'): 18,
    ('g', 'minor'): 19, ('g#', 'minor'): 20,
    ('ab', 'minor'): 20, ('a', 'minor'): 21,
    ('a#', 'minor'): 22, ('bb', 'minor'): 22,
    ('b', 'minor'): 23, ('h', 'minor'): 23
}


def normalize_key_label(key_label):
        annotation = key_label.strip().split()
        key_index = -1
        if len(annotation) == 1:
            key = annotation[0]
            if key in keys_single_letter:
                key_index = keys_single_letter[key]
        elif len(annotation) == 2:
            key = (annotation[0].lower(), annotation[1].lower())
            if key in keys_duple:
                key_index = keys_duple[key]
        # if key_index == -1:
        #     print('Did not recognize the format of label {}'.format(annotation))
        key = keys_single_reverse[key_index]
        return key



if __name__ == '__main__':
    dataset_folder = sys.argv[1]
    new_folder = sys.argv[1] + '_inplace'
    annotations = os.path.join(dataset_folder, 'annotations')
    features =os.path.join(dataset_folder, 'features')
    for f in os.listdir(features):
        base = os.path.basename(f)
        noext, dotext = os.path.splitext(base)
        keyfile = os.path.join(annotations, noext + '.key')
        with open(keyfile) as k:
            original_key = k.read().strip()
            key = normalize_key_label(original_key)
        new_filename = '{}_{}{}'.format(noext, key, dotext)
        if not os.path.exists(new_folder):
            os.makedirs(new_folder)
        copyfile(
            os.path.join(features, f), 
            os.path.join(new_folder, new_filename)
        )
        