/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
  NNLS-Chroma / Chordino

  Audio feature extraction plugins for chromagram and chord
  estimation.

  Centre for Digital Music, Queen Mary University of London.
  This file copyright 2008-2010 Matthias Mauch and QMUL.
    
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.  See the file
  COPYING included with this distribution for more information.
*/

#ifndef _CHROMA_METHODS_H_
#define _CHROMA_METHODS_H_

#include <vector>
#include <string>

const int nBPS = 3; // bins per semitone
const int nOctave = 7;
const int nNote = nOctave * 12 * nBPS + 2 * (nBPS/2+1); // a core over all octaves, plus some overlap at top and bottom
const int MIDI_basenote = 45;

extern std::vector<float> SpecialConvolution(std::vector<float> convolvee, std::vector<float> kernel);
extern void dictionaryMatrix(float* dm, float s_param);
extern std::vector<std::string> chordDictionary(std::vector<float> *mchorddict, std::vector<std::vector<int> > *m_chordnotes, float boostN, float harte_syntax);
extern bool logFreqMatrix(int fs, int blocksize, float *outmatrix);

static const float basswindow[] = {0.001769, 0.015848, 0.043608, 0.084265, 0.136670, 0.199341, 0.270509, 0.348162, 0.430105, 0.514023, 0.597545, 0.678311, 0.754038, 0.822586, 0.882019, 0.930656, 0.967124, 0.990393, 0.999803, 0.995091, 0.976388, 0.944223, 0.899505, 0.843498, 0.777785, 0.704222, 0.624888, 0.542025, 0.457975, 0.375112, 0.295778, 0.222215, 0.156502, 0.100495, 0.055777, 0.023612, 0.004909, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000};
static const float treblewindow[] = {0.000350, 0.003144, 0.008717, 0.017037, 0.028058, 0.041719, 0.057942, 0.076638, 0.097701, 0.121014, 0.146447, 0.173856, 0.203090, 0.233984, 0.266366, 0.300054, 0.334860, 0.370590, 0.407044, 0.444018, 0.481304, 0.518696, 0.555982, 0.592956, 0.629410, 0.665140, 0.699946, 0.733634, 0.766016, 0.796910, 0.826144, 0.853553, 0.878986, 0.902299, 0.923362, 0.942058, 0.958281, 0.971942, 0.982963, 0.991283, 0.996856, 0.999650, 0.999650, 0.996856, 0.991283, 0.982963, 0.971942, 0.958281, 0.942058, 0.923362, 0.902299, 0.878986, 0.853553, 0.826144, 0.796910, 0.766016, 0.733634, 0.699946, 0.665140, 0.629410, 0.592956, 0.555982, 0.518696, 0.481304, 0.444018, 0.407044, 0.370590, 0.334860, 0.300054, 0.266366, 0.233984, 0.203090, 0.173856, 0.146447, 0.121014, 0.097701, 0.076638, 0.057942, 0.041719, 0.028058, 0.017037, 0.008717, 0.003144, 0.000350};


#include "nnls.h"

#endif

