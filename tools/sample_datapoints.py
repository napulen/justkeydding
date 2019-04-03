import os
import random
import math

test_size = 0.2
no_extension = True

files = os.listdir('.')
k = math.floor(len(files) * test_size)
sample = random.sample(files, k)

for x in files:
    if x not in sample:
        if no_extension:
            x = x[:-4]
        print(x)

print('\n\n\n\n')

for x in sample:
    if no_extension:
        x = x[:-4]
    print(x)