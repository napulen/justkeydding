#!/bin/bash

find $1 -type f -iname "*.wav" -print0 | while IFS= read -r -d $'\0' filename; do
    echo "${filename::-4}".csv
    nnls-chroma-1.1/chromaextract $filename > "${filename::-4}".csv
done
