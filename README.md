# Justkeydding

Install through pip

```
pip install justkeydding
```

After installing, you can run the `justkeydding` module itself to compute outputs:

```
# Getting global key of an input
python -m justkeydding /path/to/input

# Getting the local keys
python -m justkeydding /path/to/input --local

# Annotate the file
python -m justkeydding /path/to/input --local --annotate
```

Supported input files are:

- `symbolic`: MusicXML, Humdrum(**kern), MEI, ABC (all handled through `music21`)
- `midi`: MIDI and/or MID (handled through `mido`)
- `audio`: WAV (handled through `madmom`)

Currently, `--annotate` only works for symbolic inputs.