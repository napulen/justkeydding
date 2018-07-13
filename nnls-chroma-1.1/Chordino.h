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

#ifndef _CHORDINO_
#define _CHORDINO_

#include "NNLSBase.h"

using namespace std;


class Chordino : public NNLSBase
{
public:
    Chordino(float inputSampleRate);
    virtual ~Chordino();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;

    ParameterList getParameterDescriptors() const;
    OutputList getOutputDescriptors() const;

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);
    FeatureSet getRemainingFeatures();

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

protected:
    mutable int m_outputChords;
    mutable int m_outputChordnotes;
    mutable int m_outputHarmonicChange;
    mutable int m_outputLoglikelihood;
    vector<float> m_chorddict;
    vector<vector<int> > m_chordnotes;
    vector<string> m_chordnames;
};



#endif
