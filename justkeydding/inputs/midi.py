import mido

supported_extensions = ['mid', 'midi']

def parse_file(filename):
    """ Returns a list of notes from the note_on events of a MIDI file. """
    mid = mido.MidiFile(filename)
    slices = []
    slic = []
    for msg in mid:
        if msg.type == 'note_on' and msg.velocity > 0:
            if msg.time > 0:
                slices.append(slic)
                slic = [msg.note % 12]
            else:
                slic.append(msg.note % 12)
    if slic:
        slices.append(slic)
    return slices