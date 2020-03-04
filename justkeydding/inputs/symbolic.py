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


def annotate_local_keys(filename, local_keys):
    s = music21.converter.parse(filename)
    c = s.chordify()
    c.partName = 'Local keys'
    c.partAbbreviation = 'Local keys'
    s.append(c)
    events = c.flat.notesAndRests
    if len(events) != len(local_keys):
        print(len(events), len(local_keys))
        raise ValueError('Expected same slice/local keys length.')
    for idx, ev in enumerate(c.flat.notesAndRests):
        if type(ev) == music21.chord.Chord:
            key = local_keys[idx]
            ev._notes = [music21.note.Note(key)]
            ev.addLyric(key)
        else:
            ev.addLyric('')
    s.show()
