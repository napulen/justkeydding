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

#include "Tuning.h"

#include "chromamethods.h"

#include <cstdlib>
#include <fstream>
#include <cmath>

#include <algorithm>

const bool debug_on = false;

Tuning::Tuning(float inputSampleRate) :
    NNLSBase(inputSampleRate)
{
    if (debug_on) cerr << "--> Tuning" << endl;
}

Tuning::~Tuning()
{
    if (debug_on) cerr << "--> ~Tuning" << endl;
}

size_t 
Tuning::getPreferredStepSize() const
{
    if (debug_on) cerr << "--> getPreferredStepSize" << endl;
    return 2048*4; 
}

string
Tuning::getIdentifier() const
{
    if (debug_on) cerr << "--> getIdentifier" << endl;
    return "tuning";
}

string
Tuning::getName() const
{
    if (debug_on) cerr << "--> getName" << endl;
    return "Tuning";
}

string
Tuning::getDescription() const
{
    // Return something helpful here!
    if (debug_on) cerr << "--> getDescription" << endl;
    return "The tuning plugin can estimate the local and global tuning of piece. The same tuning method is used for the NNLS Chroma and Chordino plugins.";
}

Tuning::ParameterList
Tuning::getParameterDescriptors() const
{
    if (debug_on) cerr << "--> getParameterDescriptors" << endl;
    ParameterList list;

    ParameterDescriptor d0;
    d0.identifier = "rollon";
    d0.name = "bass noise threshold";
    d0.description = "Consider the cumulative energy spectrum (from low to high frequencies). All bins below the first bin whose cumulative energy exceeds the quantile [bass noise threshold] x [total energy] will be set to 0. A threshold value of 0 means that no bins will be changed.";
    d0.unit = "%";
    d0.minValue = 0;
    d0.maxValue = 5;
    d0.defaultValue = 0;
    d0.isQuantized = true;
	d0.quantizeStep = 0.5;
    list.push_back(d0);


    return list;
}

Tuning::OutputList
Tuning::getOutputDescriptors() const
{
    if (debug_on) cerr << "--> getOutputDescriptors" << endl;
    OutputList list;
    
    int index = 0;

    OutputDescriptor d0;
    d0.identifier = "tuning";
    d0.name = "Tuning";
    d0.description = "Returns a single label (at time 0 seconds) containing an estimate of the concert pitch in Hz.";
    d0.unit = "Hz";
    d0.hasFixedBinCount = true;
    d0.binCount = 1;
    d0.hasKnownExtents = true;
    d0.minValue = 427.47;
    d0.maxValue = 452.89;
    d0.isQuantized = false;
    d0.sampleType = OutputDescriptor::VariableSampleRate;
    d0.hasDuration = true;
    list.push_back(d0);
    m_outputTuning = index++;
	
    OutputDescriptor d10;
    d10.identifier = "localtuning";
    d10.name = "Local Tuning";
    d10.description = "Returns a tuning estimate at every analysis frame, an average of the (recent) previous frame-wise estimates of the concert pitch in Hz.";
    d10.unit = "Hz";
    d10.hasFixedBinCount = true;
    d10.binCount = 1;
    d10.hasKnownExtents = true;
    d10.minValue = 427.47;
    d10.maxValue = 452.89;
    d10.isQuantized = false;
    d10.sampleType = OutputDescriptor::FixedSampleRate;
    d10.sampleRate = (m_stepSize == 0) ? m_inputSampleRate/2048 : m_inputSampleRate/m_stepSize;
    d10.hasDuration = false;
    list.push_back(d10);
    m_outputLocalTuning = index++;
  
    return list;
}


bool
Tuning::initialise(size_t channels, size_t stepSize, size_t blockSize)
{	
    if (debug_on) {
        cerr << "--> initialise";
    }
	
    if (!NNLSBase::initialise(channels, stepSize, blockSize)) {
        return false;
    }

    return true;
}

void
Tuning::reset()
{
    if (debug_on) cerr << "--> reset";
    NNLSBase::reset();
}

Tuning::FeatureSet
Tuning::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{   
    if (debug_on) cerr << "--> process" << endl;

    NNLSBase::baseProcess(inputBuffers, timestamp);
	
    Feature f10; // local tuning
    f10.hasTimestamp = true;
    f10.timestamp = timestamp;
    float normalisedtuning = m_localTuning[m_localTuning.size()-1];
    float tuning440 = 440 * pow(2,normalisedtuning/12);
    f10.values.push_back(tuning440);
	
    FeatureSet fs;
    fs[m_outputLocalTuning].push_back(f10);
    return fs;	
}

Tuning::FeatureSet
Tuning::getRemainingFeatures()
{
    if (debug_on) cerr << "--> getRemainingFeatures" << endl;
    FeatureSet fsOut;
    if (m_logSpectrum.size() == 0) return fsOut;

    // 
    /**  Calculate Tuning
         calculate tuning from (using the angle of the complex number defined by the 
         cumulative mean real and imag values)
    **/
    
    float meanTuningImag = 0;
    float meanTuningReal = 0;
    for (int iBPS = 0; iBPS < nBPS; ++iBPS) {
        meanTuningReal += m_meanTunings[iBPS] * cosvalues[iBPS];
        meanTuningImag += m_meanTunings[iBPS] * sinvalues[iBPS];
    }
    

    float cumulativetuning = 440 * pow(2,atan2(meanTuningImag, meanTuningReal)/(24*M_PI));
		    
    char buffer0 [50];
		
    sprintf(buffer0, "%0.1f Hz", cumulativetuning);
		    
    // push tuning to FeatureSet fsOut
    Feature f0; // tuning
    f0.hasTimestamp = true;
    f0.timestamp = Vamp::RealTime::frame2RealTime(0, lrintf(m_inputSampleRate));
    f0.values.push_back(cumulativetuning);
    f0.label = buffer0;
    f0.hasDuration = true;
    f0.duration = m_logSpectrum[m_logSpectrum.size()-1].timestamp;
    fsOut[m_outputTuning].push_back(f0);  
		    
    return fsOut;     

}

