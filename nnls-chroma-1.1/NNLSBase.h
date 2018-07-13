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

#ifndef _NNLS_BASE_
#define _NNLS_BASE_

#include <vamp-sdk/Plugin.h>
#include <list>

using namespace std;

class NNLSBase : public Vamp::Plugin
{
public:
    virtual ~NNLSBase();

    string getMaker() const;
    int getPluginVersion() const;
    string getCopyright() const;

    InputDomain getInputDomain() const;
    size_t getPreferredBlockSize() const;
    size_t getPreferredStepSize() const;
    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(string identifier) const;
    void setParameter(string identifier, float value);

    ProgramList getPrograms() const;
    string getCurrentProgram() const;
    void selectProgram(string name);

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

protected:
    NNLSBase(float inputSampleRate);
    void baseProcess(const float *const *inputBuffers,
                     Vamp::RealTime timestamp);

    int m_frameCount;
    FeatureList m_logSpectrum;
    size_t m_blockSize;
    size_t m_stepSize;
    int m_lengthOfNoteIndex;
    vector<float> m_meanTunings;
    vector<float> m_localTunings;
    float m_whitening;
    float m_preset;
	float m_useNNLS;
    vector<float> m_localTuning;
    vector<float> m_kernelValue;
    vector<int> m_kernelFftIndex;
    vector<int> m_kernelNoteIndex;
    float *m_dict;
    bool m_tuneLocal;
    float m_doNormalizeChroma;
    float m_rollon;
    float m_boostN;
    float m_s;
	float m_harte_syntax;
    vector<float> hw;
    vector<float> sinvalues;
    vector<float> cosvalues;
};



#endif
