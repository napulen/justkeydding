#!/bin/bash

find . | grep "01-a-maps_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_01-a-maps_chroma_inplace

find . | grep "02-a-maps_symbolic_inplace.*\.mid" > midi_dataset.txt
python3 ga.py
mv logs logs_02-a-maps_symbolic_inplace

find . | grep "03-albrecht_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_03-albrecht_chroma_inplace

find . | grep "04-albrecht_symbolic_inplace.*\.mid" > midi_dataset.txt
python3 ga.py
mv logs logs_04-albrecht_symbolic_inplace

find . | grep "05-arthur2018-recordings_chroma.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_05-arthur2018-recordings_chroma

find . | grep "06-arthur2018-synth-shortname_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_06-arthur2018-synth-shortname_chroma_inplace

find . | grep "07-arthur2018-synth-shortname_symbolic_inplace.*\.mid" > midi_dataset.txt
python3 ga.py
mv logs logs_07-arthur2018-synth-shortname_symbolic_inplace

find . | grep "08-billboard_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_08-billboard_chroma_inplace

find . | grep "09-cross-era-shortname_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_09-cross-era-shortname_chroma_inplace

find . | grep "10-giantsteps-key_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_10-giantsteps-key_chroma_inplace

find . | grep "11-giantsteps-mtg-key_chroma_inplace.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_11-giantsteps-mtg-key_chroma_inplace

find . | grep "12-napoles2018_chroma.*\.csv" > midi_dataset.txt
python3 ga.py
mv logs logs_12-napoles2018_chroma

find . | grep "13-napoles2018_symbolic.*\.mid" > midi_dataset.txt
python3 ga.py
mv logs logs_13-napoles2018_symbolic

find . | grep "^\.\/[0-9][0-9]-.*[\.mid|\.csv]$" > midi_dataset.txt
python3 ga.py
mv logs logs-14-alldatasets