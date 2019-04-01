#!/bin/bash

for f in $(cat val.txt)
do
    trimmed=$(echo $f | cut -d '_' -f 2 | cut -d '-' -f 1)
    echo "$trimmed.csv" >> val_trimmed.txt
done