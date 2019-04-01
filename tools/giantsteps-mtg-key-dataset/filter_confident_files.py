import os

annotations = 'annotations'
features = 'features'

allowed_files = []

for f in os.listdir(annotations):
    no_extension = f.split('.')[0]
    allowed_files.append(no_extension)

for f in os.listdir(features):
    no_extension = f.split('.')[0]
    if no_extension not in allowed_files:
        os.remove(os.path.join(features, f))
