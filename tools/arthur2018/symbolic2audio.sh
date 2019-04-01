#!/bin/bash

musescore=/c/Program\ Files/MuseScore\ 3/bin/MuseScore3.exe

for f in $(ls)
do
    "$musescore" -o "${f: 0:-4}.wav" "$f"
done