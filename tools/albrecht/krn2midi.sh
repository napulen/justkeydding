#!/bin/bash

for f in $(ls)
do
    midiname="${f: 0:-4}.mid"
    echo $f
    ~/humdrum-tools/humextra/bin/hum2mid $f -o $midiname
done