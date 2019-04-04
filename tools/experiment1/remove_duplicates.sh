#!/bin/bash

for f in $(cat removed_duplicates.txt)
do
    rm "experiment1_symbolic/features/$f.mid"
    rm "experiment1_chroma/features/$f.csv"
    rm "experiment1_symbolic/annotations/$f.key"
    rm "experiment1_chroma/annotations/$f.key"
done