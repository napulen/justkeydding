#!/bin/bash

for f in $(ls)
do
    annotation=$(echo ${f##*_} | cut -d '.' -f 1)
    echo "$annotation" > "${f: 0:-4}.key"
done