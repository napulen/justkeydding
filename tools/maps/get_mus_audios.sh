#!/bin/bash

output=features

for f in $(find . | grep "MUS" | grep ".wav")
do
    cp $f $output/
done