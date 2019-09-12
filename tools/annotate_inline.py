import os
import sys

keys_single_letter = {
    'C': 0, 'C#': 1, 'Db': 1, 'D': 2,
    'D#': 3, 'Eb': 3, 'E': 4, 'F': 5,
    'F#': 6, 'Gb': 6, 'G': 7, 'G#': 8,
    'Ab': 8, 'A': 9, 'A#': 10, 'Bb': 10,
    'B': 11,

    'c': 12, 'c#': 13, 'db': 13, 'd': 14,
    'd#': 15, 'eb': 15, 'e': 16, 'f': 17,
    'f#': 18, 'gb': 18, 'g': 19, 'g#': 20,
    'ab': 20, 'a': 21, 'a#': 22, 'bb': 22,
    'b': 23
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
    ('b', 'major'): 11,

    ('c', 'minor'): 12, ('c#', 'minor'): 13,
    ('db', 'minor'): 13, ('d', 'minor'): 14,
    ('d#', 'minor'): 15, ('eb', 'minor'): 15,
    ('e', 'minor'): 16, ('f', 'minor'): 17,
    ('f#', 'minor'): 18, ('gb', 'minor'): 18,
    ('g', 'minor'): 19, ('g#', 'minor'): 20,
    ('ab', 'minor'): 20, ('a', 'minor'): 21,
    ('a#', 'minor'): 22, ('bb', 'minor'): 22,
    ('b', 'minor'): 23
}



if __name__ == '__main__':
    dataset_folder = sys.argv[1]
    new_folder = sys.argv[1] + '_inline'
    annotations = os.path.join(dataset_folder, 'annotations')
    features =os.path.join(dataset_folder, 'features')
    for f in os.listdir(features):
        base = os.path.basename(f)
        noext, dotext = os.path.splitext(base)
        keyfile = os.path.join(annotations, noext + '.key')
        with open(keyfile) as k:
            key = k.read()
        