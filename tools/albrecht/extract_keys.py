import os
import csv
import re
import codecs

dataset_location = '.'

for f in os.listdir(dataset_location):
    print(f)
    fkey = '{}.key'.format(f[:-4])
    key_filepath = os.path.join(dataset_location, fkey)
    filepath = os.path.join(dataset_location, f)
    with open(key_filepath, 'w') as new_file:
        with codecs.open(filepath, encoding='utf-8', errors='ignore') as old_file:
            kern = csv.reader(old_file, delimiter='\t')
            for row in kern:
                first_spine = row[0]
                match = re.search(r"\*(.*):$", first_spine)
                if match:
                    print(match.group(1))
                    continue
