#!/bin/bash

for f in $(ls)
do
 trimmed=$(echo $f | cut -d '_' -f 1)
 mv $f "$trimmed.csv"
done