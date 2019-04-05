#!/bin/bash

i=0

for f in $(ls)
do
    id=$(printf %04d $i)
    mv "$f" "${id}__${f}"
    ((i++))
done