#!/bin/bash

for f in $(ls)
do
    newext=${f: 0:-4}.key
    mv "$f" "$newext"
done