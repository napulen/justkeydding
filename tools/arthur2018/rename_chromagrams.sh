#!/bin/bash

for f in $(ls | grep ".wav")
do
    filename=$(echo ${f: 0:-4})
    nnlschroma="vamp_nnls-chroma_nnls-chroma_chroma"
    mv "${filename}_${nnlschroma}.csv" "${filename}.csv"
done