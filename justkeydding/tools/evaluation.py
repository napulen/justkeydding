import justkeydding.parameters


def get_key_from_filename(filename):
    """Returns the key of a midi file if it is a postfix of the filename"""
    key = filename[:-4].split('_')[-1]
    keys_plus_enharmonics = list(justkeydding.parameters.states) + list(justkeydding.parameters.enharmonics.values())
    return key if key in keys_plus_enharmonics else 'x'


def is_key_guess_correct(ground_truth, guess):
    """Returns whether a key guess is correct or not"""
    if ground_truth in justkeydding.parameters.states:
        iscorrect = True if ground_truth == guess else False
    elif ground_truth in list(justkeydding.parameters.enharmonics.values()):
        if guess in justkeydding.parameters.enharmonics and ground_truth == justkeydding.parameters.enharmonics[guess]:
            iscorrect = True
        else:
            iscorrect = False
    return iscorrect
