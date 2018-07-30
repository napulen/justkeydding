#!/bin/bash

find $1 -type f -iname "*.wav" -print0 | while IFS= read -r -d $'\0' filename; do
    echo -n "$filename,"
    echo $(bin/justkeydding "$filename")
done