#!/bin/bash

for f in $(cat test.txt)
do
    trimmed=$(echo $f | cut -d '_' -f 2 | cut -d '-' -f 1)
    echo "$trimmed.csv" >> test_trimmed.txt
done