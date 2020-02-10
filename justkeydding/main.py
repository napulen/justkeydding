"""justkeydding

Creates the parameters for a Hidden Markov Model
that finds the key of a sequence of notes

Nestor Napoles (napulen@gmail.com)
"""

import optimizer.key_transitions as kt
import optimizer.key_profiles as kp
import mido
import music21
import pprint as pp
import numpy as np
import os
import argparse

states = (
    'C', 'Db', 'D', 'Eb', 'E', 'F', 'F#', 'G', 'Ab', 'A', 'Bb', 'B',
    'c', 'c#', 'd', 'eb', 'e', 'f', 'f#', 'g', 'ab', 'a', 'bb', 'b',
)

enharmonics = {
    'Db': 'C#', 'Eb': 'D#', 'F#': 'Gb', 'Ab': 'G#', 'Bb': 'A#',
    'c#': 'db', 'eb': 'd#', 'f#': 'gb', 'ab': 'g#', 'bb': 'a#',
}

start_p = {
    'C': 1.0/24.0, 'Db': 1.0/24.0, 'D': 1.0/24.0, 'Eb': 1.0/24.0,
    'E': 1.0/24.0, 'F': 1.0/24.0, 'F#': 1.0/24.0, 'G': 1.0/24.0,
    'Ab': 1.0/24.0, 'A': 1.0/24.0, 'Bb': 1.0/24.0, 'B': 1.0/24.0,
    'c': 1.0/24.0, 'c#': 1.0/24.0, 'd': 1.0/24.0, 'eb': 1.0/24.0,
    'e': 1.0/24.0, 'f': 1.0/24.0, 'f#': 1.0/24.0, 'g': 1.0/24.0,
    'ab': 1.0/24.0, 'a': 1.0/24.0, 'bb': 1.0/24.0, 'b': 1.0/24.0,
}


def create_transition_probabilities(key_transitions):
    """Returns the transition probabilities"""
    d = dict()
    for idx, key in enumerate(states):
        tonic = key_transitions[:12]
        relative = key_transitions[12:]
        tonic_rotation = -(idx % 12)
        relative_rotation = -(idx % 12)
        if idx >= 12:
            tonic, relative = relative, tonic
            tonic_rotation = (tonic_rotation - 3) % 12
        probs1 = tonic[tonic_rotation:] + tonic[:tonic_rotation]
        probs2 = relative[relative_rotation:] + relative[:relative_rotation]
        kt_ = probs1 + probs2
        d[key] = {key: kt_[idx] for idx, key in enumerate(states)}
    return d


def create_emission_probabilities(major, minor):
    """Returns the emission probabilities"""
    d = dict()
    for idx, key in enumerate(states):
        rotation = -(idx % 12)
        profile = major if idx < 12 else minor
        profile = profile[rotation:] + profile[:rotation]
        d[key] = {pc: profile[pc] for pc in range(12)}
    return d


def get_notes_from_midi(midi_file):
    """Returns a list of notes from the note_on events of a MIDI file"""
    mid = mido.MidiFile(midi_file)
    notes = [msg.note for msg in mid
             if msg.type == 'note_on'
             and msg.velocity > 0]
    return notes


def get_notes_chords_and_rests_from_music21(m21_input_file):
    s = music21.converter.parse(m21_input_file)
    notes_and_rests = s.parts.flat.notesAndRests
    return notes_and_rests


def get_notes_from_music21(notes_chords_and_rests):
    notes = []
    for c in notes_chords_and_rests:
        if isinstance(c, music21.chord.Chord):
            for n in c:
                notes.append(n)
        elif isinstance(c, music21.note.Note):
            notes.append(c)
    return notes


def get_note_names_from_music21(notes):
    return [n.name for n in notes]


def get_pc_from_music21(notes):
    return [n.pitch.pitchClass for n in notes]


def get_pc_from_midi_notes(notes):
    """Returns the list of pitch-classes from a list of midi notes"""
    return [note % 12 for note in notes]


def extract_input_sequence(input_file, is_sequence=False):
    if is_sequence == True:
        input_sequence = [int(s) for s in input_file.split(',')]
    elif input_file.endswith('.mid') or input_file.endswith('.midi'):
        notes = get_notes_from_midi(input_file)
        input_sequence = get_pc_from_midi_notes(notes)
    elif input_file.endswith('.musicxml') or input_file.endswith('.xml') or input_file.endswith('.mei') or input_file.endswith('.krn'):
        notes_chords_and_rests = get_notes_chords_and_rests_from_music21(input_file)
        notes = get_notes_from_music21(notes_chords_and_rests)
        input_sequence = get_pc_from_music21(notes)
    return input_sequence


def mylog(x):
    """Returns the logarithm of x (without the annoying warnings of np.log)"""
    return np.log(x) if x > 8.7565e-27 else -np.inf


def get_key_from_filename(filename):
    """Returns the key of a midi file if it is a postfix of the filename"""
    key = filename[:-4].split('_')[-1]
    keys_plus_enharmonics = list(states) + list(enharmonics.values())
    return key if key in keys_plus_enharmonics else 'x'


def is_key_guess_correct(ground_truth, guess):
    """Returns whether a key guess is correct or not"""
    if ground_truth in states:
        iscorrect = True if ground_truth == guess else False
    elif ground_truth in list(enharmonics.values()):
        iscorrect = True if guess in enharmonics and ground_truth == enharmonics[guess] else False
    return iscorrect


def viterbi(obs, states, start_p, trans_p, emit_p):
    V = [{}]
    for st in states:
        V[0][st] = {
            "prob": mylog(start_p[st])
            + mylog(emit_p[st][obs[0]]), "prev": None
            }
    # Run Viterbi when t > 0
    for t in range(1, len(obs)):
        V.append({})
        for st in states:
            max_tr_prob = max(V[t-1][prev_st]["prob"] + mylog(trans_p[prev_st][st]) for prev_st in states)
            for prev_st in states:
                if V[t-1][prev_st]["prob"] + mylog(trans_p[prev_st][st]) == max_tr_prob:
                    max_prob = max_tr_prob + mylog(emit_p[st][obs[t]])
                    V[t][st] = {"prob": max_prob, "prev": prev_st}
                    break
    # for line in dptable(V):
    #    print(line)
    opt = []
    # The highest probability
    max_prob = max(value["prob"] for value in V[-1].values())
    previous = None
    # Get most probable state and its backtrack
    for st, data in V[-1].items():
        if data["prob"] == max_prob:
            opt.append(st)
            previous = st
            break
    # Follow the backtrack till the first observation
    for t in range(len(V) - 2, -1, -1):
        opt.insert(0, V[t + 1][previous]["prev"])
        previous = V[t + 1][previous]["prev"]

    # print('The steps of states are '
    #     + ' '.join(opt)
    #      + ' with highest probability of %s' % max_prob)
    return opt, max_prob

def analyze(input_sequence, kp_major_name, kp_minor_name, kt_name):
    # Preparing the args for the first HMM
    key_transition = kt._kt[kt_name]
    trans_p = create_transition_probabilities(key_transition)
    major = kp._kp[kp_major_name][:12]
    minor = kp._kp[kp_minor_name][12:]
    emit_p = create_emission_probabilities(major, minor)
    obs = input_sequence
    local_keys, max_p = viterbi(obs, states, start_p, trans_p, emit_p)
    # if args.output_local:
    #     print(local_keys)
    #     return
    # Preparing the args for the second HMM
    obs = local_keys  # the keys become the observations
    emit_p = trans_p  # the transitions become emission
    key_transitions = kt._kt["null"]
    trans_p = create_transition_probabilities(key_transitions)
    key, max_prob = viterbi(obs, states, start_p, trans_p, emit_p)
    global_key = key[0]
    return [global_key, local_keys]

def batch(args):
    transitions = [
        'ktg_exponential10',
        'ktg_exponential2',
    ]
    profiles_major = [
        'krumhansl_kessler',
        'aarden_essen',
        'sapp',
        'bellman_budge',
        'temperley'
    ]
    profiles_minor = [
        'krumhansl_kessler',
        'aarden_essen',
        'sapp',
        'bellman_budge',
        'temperley'
    ]
    scores = {}
    for transition in transitions:
        for profile_major in profiles_major:
            for profile_minor in profiles_minor:
                print("Hidden Markov Model parameters:\n"
                      "key_transitions: {}\n"
                      "key_profile (major): {}\n"
                      "key_profile (minor): {}\n"
                      "Filename\tOriginal\tGuess\t"
                      "Correct?".format(transition, profile_major, profile_minor)
                      )
                score = 0
                for root, dirs, files in os.walk(args.input):
                    for filename in files:
                        filepath = os.path.join(root, filename)
                        ground_truth_key = get_key_from_filename(filename)
                        # Preparing the args for the first HMM
                        key_transitions = kt._kt[transition]
                        trans_p = create_transition_probabilities(key_transitions)
                        major = kp._kp[profile_major]
                        minor = kp._kp[profile_minor]
                        emit_p = create_emission_probabilities(major, minor)
                        # obs = create_observation_list(filepath)
                        state_list, max_p = viterbi(obs, states, start_p, trans_p, emit_p)

                        # Preparing the args for the second HMM
                        obs = state_list  # the keys become the observations
                        emit_p = trans_p  # the transitions become emission
                        key_transitions = kt._kt["null"]
                        trans_p = create_transition_probabilities(key_transitions)
                        key, max_prob = viterbi(obs, states, start_p, trans_p, emit_p)
                        guess_key = key[0]
                        iscorrect = is_key_guess_correct(ground_truth_key, guess_key)
                        score += 0 if iscorrect else 1

                        print('{}:\t{}\t{}\t{}'.format(filepath,
                                                       ground_truth_key,
                                                       guess_key,
                                                       "Good" if iscorrect else "Wrong"))
                scores[(transition, profile_major, profile_minor)] = score
    pp.pprint(scores)


def parse_args():
    parser = argparse.ArgumentParser(description='justkeydding for symbolic music files, python version')
    parser.add_argument(
        'input',
        help='Input symbolic music file (or folder if --batch)'
    )
    parser.add_argument(
        '--batch',
        dest='is_batch',
        const=True,
        action='store_const',
        help='Process several files within a folder'
    )
    parser.add_argument(
        '--sequence',
        dest='is_sequence',
        const=True,
        action='store_const',
        default=False,
        help='Provide the input as a string of comma-separated pitch-classes'
    )
    parser.add_argument(
        '--local',
        dest='output_local',
        const=True,
        action='store_const',
        help='Output local keys'
    )
    parser.add_argument(
        '--transition',
        dest='key_transition',
        choices=[
            'ktg_exponential10',
            'ktg_exponential2'
        ],
        default='ktg_exponential10',
        help='Key transition to use'
    )
    parser.add_argument(
        '--majorEmission',
        dest='key_profile_major',
        choices=[
            'krumhansl_kessler',
            'aarden_essen',
            'sapp',
            'bellman_budge',
            'temperley',
        ],
        default='sapp',
        help='Major key profile to use as emission probability distribution'
    )
    parser.add_argument(
        '--minorEmission',
        dest='key_profile_minor',
        choices=[
            'krumhansl_kessler',
            'aarden_essen',
            'sapp',
            'bellman_budge',
            'temperley',
        ],
        default='sapp',
        help='Minor key profile to use as emission probability distribution'
    )
    args = parser.parse_args()
    return args


if __name__ == '__main__':
    args = parse_args()
    # print(args)
    if args.is_batch:
        batch(args)
    else:
        input_sequence = extract_input_sequence(args.input, args.is_sequence)
        outputs = analyze(input_sequence, args.key_profile_major, args.key_profile_minor, args.key_transition)
        if args.output_local:
            print(outputs)
        else:
            print(outputs[0])

