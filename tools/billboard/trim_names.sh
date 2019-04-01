#!/bin/bash

for f in $(ls)
do
 trimmed=$(echo $f | cut -d '_' -f 2 | cut -d '-' -f 1)
 mv $f "$trimmed.key"
done