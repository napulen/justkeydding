#!/bin/bash

find $1 -type f -iname "*.csv" -print0 | while IFS= read -r -d $'\0' filename; do
    echo -n "$filename,"
    echo $(bin/justkeydding $2 -f csv "$filename")
done
