import music21

supported_extensions = ['mxl', 'musicxml', 'xml', 'krn', 'mei']

def parse_file(filename):
    s = music21.converter.parse(filename)
    c = s.chordify()
    slices = []
    for ev in c.flat.notesAndRests:
        if type(ev) == music21.chord.Chord:
            slic = [n.pitch.pitchClass for n in ev]
        elif type(ev) == music21.note.Note:
            slic = [ev.pitch.pitchClass]
        else:
            slic = []
        slices.append(slic)
    return slices
