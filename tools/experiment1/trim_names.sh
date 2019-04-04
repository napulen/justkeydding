#!/bin/bash

for f in $(ls)
do
    id=$(echo $f | cut -d "_" -f 1)
    mv "$f" "$id.${f: -3}"
done