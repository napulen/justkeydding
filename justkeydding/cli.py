import argparse


def parse_args():
    parser = argparse.ArgumentParser(
        description='justkeydding for symbolic music files, python version')
    parser.add_argument(
        'input',
        help='Input symbolic music file (or folder if --batch)'
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
    parser.add_argument(
        '--annotate',
        dest='annotate_file',
        const=True,
        action='store_const',
        help='Annotate the input file'
    )
    args = parser.parse_args()
    return args
