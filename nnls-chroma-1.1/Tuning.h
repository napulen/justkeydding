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

#ifndef _TUNING_
#define _TUNING_

#include "NNLSBase.h"

using namespace std;


class Tuning : public NNLSBase
{
public:
    Tuning(float inputSampleRate);
    virtual ~Tuning();

    string getIdentifier() const;
    string getName() const;
    string getDescription() const;
    size_t getPreferredStepSize() const;
    
    ParameterList getParameterDescriptors() const;
    OutputList getOutputDescriptors() const;

    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp);
    FeatureSet getRemainingFeatures();

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

protected:
    mutable int m_outputTuning;
    mutable int m_outputLocalTuning;
};



#endif
