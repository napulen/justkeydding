import mido
import os

unique = {}
duplicates = {}

for f in os.listdir('.'):
    mid = mido.MidiFile(f)
    notes = []
    for msg in mid:
        if msg.type != 'note_on' or msg.velocity == 0:
            continue
        notes.append(msg.note % 12)
    dup = ''
    for k,v in unique.items():
        if notes == v:
            dup = k
    if dup:
        duplicates[f] = dup
    else:
        unique[f] = notes