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

#ifndef _VITERBI_H_
#define _VITERBI_H_

#include <vector>
#include <string>
using namespace std;

extern std::vector<int> ViterbiPath(std::vector<double> init, std::vector<vector<double> > trans, std::vector<vector<double> > obs, double *delta, vector<double> *scale);

#endif