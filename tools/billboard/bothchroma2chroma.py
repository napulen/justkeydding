from tempfile import mkstemp
from shutil import move
from os import fdopen, remove
import os
import csv

dataset_location = '.'

for f in os.listdir(dataset_location):
    print(f)
    fh, abs_path = mkstemp()
    filepath = os.path.join(dataset_location, f)
    with os.fdopen(fh, 'w') as new_file:
        with open(filepath) as old_file:
            bothchroma = csv.reader(old_file)
            for row in bothchroma:
                chroma = row[1:2] + row[14:]
                new_file.write(','.join(chroma))
                new_file.write('\n')
    os.remove(filepath)
    move(abs_path, filepath)
