#!/bin/bash

# To copy all the bothchroma.csv files inside the track id's folder and remove the folders afterwards

for f in $(ls)
do
    chroma="$f/bothchroma.csv"
    cp $chroma "$f.csv"
    rm -R $f
done
