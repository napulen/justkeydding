"""justkeydding

Key-finding algorithm for Symbolic and Audio files.

Nestor Napoles (napulen@gmail.com)
"""

import justkeydding.inputs
import justkeydding.parameters
import justkeydding.parameters.key_transitions as kt
import justkeydding.parameters.key_profiles as kp
from justkeydding.parameters.observations import Observations
import justkeydding.cli

import numpy as np
from collections import Counter


def create_transition_probabilities(key_transitions):
    """Returns the transition probabilities"""
    d = dict()
    for idx, key in enumerate(justkeydding.parameters.states):
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
        d[key] = {key: kt_[idx] for idx, key in enumerate(justkeydding.parameters.states)}
    return d


def create_emission_probabilities(major, minor):
    """Returns the emission probabilities"""
    d = dict()
    for idx, key in enumerate(justkeydding.parameters.states):
        rotation = -(idx % 12)
        profile = major if idx < 12 else minor
        profile = profile[rotation:] + profile[:rotation]
        d[key] = {pc: profile[pc] for pc in range(12)}
    return d


def resolve_input_type(input_file):
    extension = input_file.rsplit('.')[-1]
    input_type = ''
    if extension in justkeydding.inputs.midi.supported_extensions:
        input_type = 'midi'
    elif extension in justkeydding.inputs.symbolic.supported_extensions:
        input_type = 'symbolic'
    elif extension in justkeydding.inputs.audio.supported_extensions:
        input_type = 'audio'
    return input_type


def extract_input_sequence(input_file, is_sequence=False):
    if is_sequence:
        # TODO: Implement this for new format of input sequences
        input_sequence = [int(s) for s in input_file.split(',')]
    else:
        input_sequence = input_format.parse_file(input_file)
    return Observations(input_sequence)


def mylog(x):
    """Returns the logarithm of x (without the annoying warnings of np.log)"""
    return np.log(x) if x > 8.7565e-27 else -np.inf


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
    return opt, max_prob


def analyze(input_sequence, kp_major_name, kp_minor_name, kt_name):
    # Preparing the args for the first HMM
    key_transition = kt._kt[kt_name]
    trans_p = create_transition_probabilities(key_transition)
    major = kp._kp[kp_major_name][:12]
    minor = kp._kp[kp_minor_name][12:]
    emit_p = create_emission_probabilities(major, minor)
    obs = input_sequence
    local_keys, max_p = viterbi(
        obs,
        justkeydding.parameters.states,
        justkeydding.parameters.start_p,
        trans_p,
        emit_p)
    sliced_local_keys = list(zip(local_keys, obs.slice_indexes()))
    sliced_local_keys = Observations.from_tuples(sliced_local_keys)
    # Preparing the args for the second HMM
    obs = local_keys  # the keys become the observations
    emit_p = trans_p  # the transitions become emission
    key_transitions = kt._kt["null"]
    trans_p = create_transition_probabilities(key_transitions)
    key, max_prob = viterbi(
        obs,
        justkeydding.parameters.states,
        justkeydding.parameters.start_p,
        trans_p,
        emit_p
    )
    global_key = key[0]
    return [global_key, sliced_local_keys]


def postprocess_local_keys(local_keys):
    slices = []
    for slic in local_keys.slices:
        c = Counter(slic)
        slices.append(max(slic, key=c.get))
    return slices


if __name__ == '__main__':
    args = justkeydding.cli.parse_args()
    input_type = resolve_input_type(args.input)
    if input_type == 'midi':
        import justkeydding.inputs.midi as input_format
    elif input_type == 'symbolic':
        import justkeydding.inputs.symbolic as input_format
    elif input_type == 'audio':
        import justkeydding.inputs.audio as input_format
    elif not args.is_sequence:
        raise ValueError("unsuported file type")
    input_sequence = extract_input_sequence(
        args.input,
        args.is_sequence
    )
    outputs = analyze(
        input_sequence,
        args.key_profile_major,
        args.key_profile_minor,
        args.key_transition
    )
    if args.output_local:
        keys_by_onset = postprocess_local_keys(outputs[1])
        print('{}\n{}'.format(outputs[0], keys_by_onset))
        # if args.annotate_file:
    else:
        print(outputs[0])
