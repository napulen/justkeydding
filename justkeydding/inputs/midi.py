import mido

supported_extensions = ['mid', 'midi']


def parse_file(filename):
    """ Returns a list of notes from the note_on events of a MIDI file. """
    mid = mido.MidiFile(filename)
    slices = []
    slic = []
    times = []
    t = 0.0
    for msg in mid:
        t += msg.time if hasattr(msg, 'time') else 0.0
        if msg.type == 'note_on' and msg.velocity > 0:
            # In case we need to start a new slice
            if not slic or msg.time > 0:
                if slic:
                    slices.append(slic)
                times.append(t)
                slic = [msg.note % 12]
            # Add to the current slice
            else:
                slic.append(msg.note % 12)
    if slic:
        slices.append(slic)
    return slices, times
