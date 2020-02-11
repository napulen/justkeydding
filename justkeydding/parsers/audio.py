import madmom.audio.chroma as chroma
import numpy as np
import numpy.linalg as linalg

supported_extensions = ['wav']

def _cosine_similarity(u, v):
    dot = np.dot(u, v)
    cos_sim = dot / (linalg.norm(u) * linalg.norm(v))
    return cos_sim

def parse_file(filename):
    deepchroma = chroma.DeepChromaProcessor()
    deepchroma = deepchroma(filename)
    similarity_threshold = 0.96
    threshold = 0.5
    prevframe = np.array([0.01] * 12)
    slices = []
    for frame in deepchroma:
        cos_sim = _cosine_similarity(frame, prevframe)
        # print(np.around(frame, 1), np.around(cos_sim, 2), end='')
        if cos_sim > similarity_threshold:
            # print(' ignoring')
            continue
        slic = np.argwhere(frame > threshold)
        slices.append(slic.reshape(1, -1)[0].tolist())
        prevframe = frame
    return slices
    
