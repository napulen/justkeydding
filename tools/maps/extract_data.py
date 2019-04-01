import mido
import os

cornercases = [
    'MAPS_MUS-bk_xmas5_AkPnBcht.mid',
    'MAPS_MUS-bk_xmas5_AkPnCGdD.mid',
    'MAPS_MUS-bk_xmas5_ENSTDkCl.mid',
    'MAPS_MUS-bk_xmas5_StbgTGd2.mid',
    'MAPS_MUS-mond_2_SptkBGAm.mid',
    'MAPS_MUS-mond_2_SptkBGCl.mid',
    'MAPS_MUS-schu_143_1_AkPnBcht.mid',
    'MAPS_MUS-schu_143_1_ENSTDkAm.mid'
]

for f in os.listdir('.'):
    print(f)
    mid = mido.MidiFile(f)
    for msg in mid:
        if msg.is_meta:
            if msg.type == 'lyric' or msg.type == 'track_name' or msg.type == 'key_signature':
                print('\t{}'.format(msg))


def extract_full_name():
    for f in sorted(os.listdir('.')):
       mid = mido.MidiFile(f)
       msgs = []
       for msg in mid:
           if msg.is_meta:
               if msg.type == 'track_name':
                   if msg.name.startswith('Copyright'):
                       print('{}, {}'.format(f, msgs[-1].name))
                       break
                   else:
                       msgs.append(msg)


def extract_key_signature():
   for f in sorted(os.listdir('.')):
       mid = mido.MidiFile(f)
       for msg in mid:
           if msg.is_meta:
               if msg.type == 'key_signature':
                   print('{}, {}'.format(f, msg.key))
                   break