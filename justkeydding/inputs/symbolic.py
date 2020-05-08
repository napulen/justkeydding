import music21

supported_extensions = ['mxl', 'musicxml', 'xml', 'krn', 'mei']


def parse_file(filename):
    s = music21.converter.parse(filename)
    c = s.chordify()    
    slices = []
    times = []
    # blacklisted_slices = _get_offsets_of_rests(s)    
    for ev in c.flat.notes:        
        # if ev.offset in blacklisted_slices:
        #     # This note was artificially generated
        #     # by chordify(), this slice only had
        #     # a rest in it. We can ignore it
        #     continue
        if type(ev) == music21.chord.Chord:
            slic = [n.pitch.pitchClass for n in ev]
        elif type(ev) == music21.note.Note:
            slic = [ev.pitch.pitchClass]
        else:
            slic = []
        slices.append(slic)
        times.append(eval(str(ev.offset)))
    return slices, times

# Rests cause all sort of alignment issues when chordify()'ed
def _get_offsets_of_rests(m21stream):
    blacklist = []
    whitelist = []
    for ev in m21stream.flat.notesAndRests:
        if type(ev) == music21.note.Rest:
            blacklist.append(ev.offset)    
        else:
            # We only care about slices where there is
            # nothing BUT a rest. Slices with rests AND notes are fine            
            whitelist.append(ev.offset)
    offsets = list(set(blacklist) - set(whitelist))
    return offsets


def annotate_local_keys(filename, local_keys):
    s = music21.converter.parse(filename)
    c = s.chordify()
    c.partName = 'Local keys'
    c.partAbbreviation = 'Local keys'
    s.append(c)
    events = c.flat.notes
    if len(events) != len(local_keys):
        print(len(events), len(local_keys))
        raise ValueError('Expected same slice/local keys length.')
    for idx, ev in enumerate(events):
        if type(ev) == music21.chord.Chord:
            key = local_keys[idx]
            ev._notes = [music21.note.Note(key)]
            ev.addLyric(key)
        else:
            ev.addLyric('')
    path, extension = tuple(filename.rsplit('.', 1))
    new_filename = path + '_justkeydding.' + extension
    s.write(fp=new_filename, fmt='musicxml')
