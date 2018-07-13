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

#include "NNLSChroma.h"

#include "chromamethods.h"

#include <cstdlib>
#include <fstream>
#include <cmath>

#include <algorithm>

const bool debug_on = false;

NNLSChroma::NNLSChroma(float inputSampleRate) :
    NNLSBase(inputSampleRate)
{
    if (debug_on) cerr << "--> NNLSChroma" << endl;
}

NNLSChroma::~NNLSChroma()
{
    if (debug_on) cerr << "--> ~NNLSChroma" << endl;
}

string
NNLSChroma::getIdentifier() const
{
    if (debug_on) cerr << "--> getIdentifier" << endl;
    return "nnls-chroma"; 
}

string
NNLSChroma::getName() const
{
    if (debug_on) cerr << "--> getName" << endl;
    return "NNLS Chroma";
}

string
NNLSChroma::getDescription() const
{
    if (debug_on) cerr << "--> getDescription" << endl;
    return "This plugin provides a number of features derived from a DFT-based log-frequency amplitude spectrum: some variants of the log-frequency spectrum, including a semitone spectrum derived from approximate transcription using the NNLS algorithm; and based on this semitone spectrum, different chroma features.";
}

NNLSChroma::OutputList
NNLSChroma::getOutputDescriptors() const
{
    if (debug_on) cerr << "--> getOutputDescriptors" << endl;
    OutputList list;
    
    // Make chroma names for the binNames property

	const char* notenames[24] = {
	    "A  (bass)","Bb (bass)","B  (bass)","C  (bass)","C# (bass)","D  (bass)","Eb (bass)","E  (bass)","F  (bass)","F# (bass)","G  (bass)","Ab (bass)",
	    "A","Bb","B","C","C#","D","Eb","E","F","F#","G","Ab"};

	
    vector<string> chromanames;
    vector<string> bothchromanames;
    for (int iNote = 0; iNote < 24; iNote++) {
        bothchromanames.push_back(notenames[iNote]);
        if (iNote < 12) {
            chromanames.push_back(notenames[iNote+12]);
        }
    }
    
    int index = 0;

    float featureRate =
        (m_stepSize == 0) ? m_inputSampleRate/2048 : m_inputSampleRate/m_stepSize;

    OutputDescriptor logfreqspecOutput;
    logfreqspecOutput.identifier = "logfreqspec";
    logfreqspecOutput.name = "Log-Frequency Spectrum";
    logfreqspecOutput.description = "A Log-Frequency Spectrum (constant Q) that is obtained by cosine filter mapping.";
    logfreqspecOutput.unit = "";
    logfreqspecOutput.hasFixedBinCount = true;
    logfreqspecOutput.binCount = nNote;
    logfreqspecOutput.hasKnownExtents = false;
    logfreqspecOutput.isQuantized = false;
    logfreqspecOutput.sampleType = OutputDescriptor::FixedSampleRate;
    logfreqspecOutput.hasDuration = false;
    logfreqspecOutput.sampleRate = featureRate;
    list.push_back(logfreqspecOutput);
    m_outputLogfreqspec = index++;

    OutputDescriptor tunedlogfreqspecOutput;
    tunedlogfreqspecOutput.identifier = "tunedlogfreqspec";
    tunedlogfreqspecOutput.name = "Tuned Log-Frequency Spectrum";
    tunedlogfreqspecOutput.description = "A Log-Frequency Spectrum (constant Q) that is obtained by cosine filter mapping, then its tuned using the estimated tuning frequency.";
    tunedlogfreqspecOutput.unit = "";
    tunedlogfreqspecOutput.hasFixedBinCount = true;
    tunedlogfreqspecOutput.binCount = nNote;
    tunedlogfreqspecOutput.hasKnownExtents = false;
    tunedlogfreqspecOutput.isQuantized = false;
    tunedlogfreqspecOutput.sampleType = OutputDescriptor::FixedSampleRate;
    tunedlogfreqspecOutput.hasDuration = false;
    tunedlogfreqspecOutput.sampleRate = featureRate;
    list.push_back(tunedlogfreqspecOutput);
    m_outputTunedlogfreqspec = index++;
    
    OutputDescriptor semitonespectrumOutput;
    semitonespectrumOutput.identifier = "semitonespectrum";
    semitonespectrumOutput.name = "Semitone Spectrum";
    semitonespectrumOutput.description = "A semitone-spaced log-frequency spectrum derived from the third-of-a-semitone-spaced tuned log-frequency spectrum.";
    semitonespectrumOutput.unit = "";
    semitonespectrumOutput.hasFixedBinCount = true;
    semitonespectrumOutput.binCount = 84;
    semitonespectrumOutput.hasKnownExtents = false;
    semitonespectrumOutput.isQuantized = false;
    semitonespectrumOutput.sampleType = OutputDescriptor::FixedSampleRate;
    semitonespectrumOutput.hasDuration = false;
    semitonespectrumOutput.sampleRate = featureRate;
    list.push_back(semitonespectrumOutput);
    m_outputSemitonespectrum = index++;
    
    OutputDescriptor chromaOutput;
    chromaOutput.identifier = "chroma";
    chromaOutput.name = "Chromagram";
    chromaOutput.description = "Tuning-adjusted chromagram from NNLS approximate transcription, with an emphasis on the medium note range.";
    chromaOutput.unit = "";
    chromaOutput.hasFixedBinCount = true;
    chromaOutput.binCount = 12;
    chromaOutput.binNames = chromanames;
    chromaOutput.hasKnownExtents = false;
    chromaOutput.isQuantized = false;
    chromaOutput.sampleType = OutputDescriptor::FixedSampleRate;
    chromaOutput.hasDuration = false;
    chromaOutput.sampleRate = featureRate;
    list.push_back(chromaOutput);
    m_outputChroma = index++;
    
    OutputDescriptor basschromaOutput;
    basschromaOutput.identifier = "basschroma";
    basschromaOutput.name = "Bass Chromagram";
    basschromaOutput.description = "Tuning-adjusted bass chromagram from NNLS approximate transcription, with an emphasis on the bass note range.";
    basschromaOutput.unit = "";
    basschromaOutput.hasFixedBinCount = true;
    basschromaOutput.binCount = 12;
    basschromaOutput.binNames = chromanames;
    basschromaOutput.hasKnownExtents = false;
    basschromaOutput.isQuantized = false;
    basschromaOutput.sampleType = OutputDescriptor::FixedSampleRate;
    basschromaOutput.hasDuration = false;
    basschromaOutput.sampleRate = featureRate;
    list.push_back(basschromaOutput);
    m_outputBasschroma = index++;
    
    OutputDescriptor bothchromaOutput;
    bothchromaOutput.identifier = "bothchroma";
    bothchromaOutput.name = "Chromagram and Bass Chromagram";
    bothchromaOutput.description = "Tuning-adjusted chromagram and bass chromagram (stacked on top of each other) from NNLS approximate transcription.";
    bothchromaOutput.unit = "";
    bothchromaOutput.hasFixedBinCount = true;
    bothchromaOutput.binCount = 24;
    bothchromaOutput.binNames = bothchromanames;
    bothchromaOutput.hasKnownExtents = false;
    bothchromaOutput.isQuantized = false;
    bothchromaOutput.sampleType = OutputDescriptor::FixedSampleRate;
    bothchromaOutput.hasDuration = false;
    bothchromaOutput.sampleRate = featureRate;
    list.push_back(bothchromaOutput);
    m_outputBothchroma = index++;    
    return list;
}


bool
NNLSChroma::initialise(size_t channels, size_t stepSize, size_t blockSize)
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
NNLSChroma::reset()
{
    if (debug_on) cerr << "--> reset";
    NNLSBase::reset();
}

NNLSChroma::FeatureSet
NNLSChroma::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{   
    if (debug_on) cerr << "--> process" << endl;

    NNLSBase::baseProcess(inputBuffers, timestamp);
	
    FeatureSet fs;
    fs[m_outputLogfreqspec].push_back(m_logSpectrum[m_logSpectrum.size()-1]);
    return fs;	
}

NNLSChroma::FeatureSet
NNLSChroma::getRemainingFeatures()
{
    
	if (debug_on) cerr << "--> getRemainingFeatures" << endl;
    FeatureSet fsOut;
    if (m_logSpectrum.size() == 0) return fsOut;

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
    float normalisedtuning = atan2(meanTuningImag, meanTuningReal)/(2*M_PI);
    int intShift = floor(normalisedtuning * 3);
    float floatShift = normalisedtuning * 3 - intShift; // floatShift is a really bad name for this
		    
    char buffer0 [50];
		
    sprintf(buffer0, "estimated tuning: %0.1f Hz", cumulativetuning);
		    		    
    /** Tune Log-Frequency Spectrogram
        calculate a tuned log-frequency spectrogram (f2): use the tuning estimated above (kinda f0) to 
        perform linear interpolation on the existing log-frequency spectrogram (kinda f1).
    **/
    if (debug_on) cerr << endl << "[NNLS Chroma Plugin] Tuning Log-Frequency Spectrogram ... ";
					
    float tempValue = 0;

    int count = 0;

		
    for (FeatureList::iterator i = m_logSpectrum.begin(); i != m_logSpectrum.end(); ++i) {
        Feature f1 = *i;
        Feature f2; // tuned log-frequency spectrum
        f2.hasTimestamp = true;
        f2.timestamp = f1.timestamp;
        f2.values.push_back(0.0); f2.values.push_back(0.0); // set lower edge to zero
		
		
        if (m_tuneLocal) {
            intShift = floor(m_localTuning[count] * 3);
            floatShift = m_localTuning[count] * 3 - intShift; // floatShift is a really bad name for this
        }
		        
        // cerr << intShift << " " << floatShift << endl;
		        
        for (int k = 2; k < (int)f1.values.size() - 3; ++k) { // interpolate all inner bins
            tempValue = f1.values[k + intShift] * (1-floatShift) + f1.values[k+intShift+1] * floatShift;
            f2.values.push_back(tempValue);
        }
		        
        f2.values.push_back(0.0); f2.values.push_back(0.0); f2.values.push_back(0.0); // upper edge

        vector<float> runningmean = SpecialConvolution(f2.values,hw);
        vector<float> runningstd;
        for (int i = 0; i < nNote; i++) { // first step: squared values into vector (variance)
            runningstd.push_back((f2.values[i] - runningmean[i]) * (f2.values[i] - runningmean[i]));
        }
        runningstd = SpecialConvolution(runningstd,hw); // second step convolve
        for (int i = 0; i < nNote; i++) { 
            runningstd[i] = sqrt(runningstd[i]); // square root to finally have running std
            if (runningstd[i] > 0) {
                // f2.values[i] = (f2.values[i] / runningmean[i]) > thresh ? 
                // 		                    (f2.values[i] - runningmean[i]) / pow(runningstd[i],m_whitening) : 0;
                f2.values[i] = (f2.values[i] - runningmean[i]) > 0 ?
                    (f2.values[i] - runningmean[i]) / pow(runningstd[i],m_whitening) : 0;
            }
            if (f2.values[i] < 0) {
                cerr << "ERROR: negative value in logfreq spectrum" << endl;
            }
        }
        fsOut[m_outputTunedlogfreqspec].push_back(f2);
        count++;
    }
    if (debug_on) cerr << "done." << endl;
	    
    /** Semitone spectrum and chromagrams
        Semitone-spaced log-frequency spectrum derived from the tuned log-freq spectrum above. the spectrum
        is inferred using a non-negative least squares algorithm.
        Three different kinds of chromagram are calculated, "treble", "bass", and "both" (which means 
        bass and treble stacked onto each other).
    **/
    if (m_useNNLS == 0) {
        if (debug_on) cerr << "[NNLS Chroma Plugin] Mapping to semitone spectrum and chroma ... ";
    } else {
        if (debug_on) cerr << "[NNLS Chroma Plugin] Performing NNLS and mapping to chroma ... ";
    }

	    
    vector<float> oldchroma = vector<float>(12,0);
    vector<float> oldbasschroma = vector<float>(12,0);
    count = 0;

    for (FeatureList::iterator it = fsOut[m_outputTunedlogfreqspec].begin(); it != fsOut[m_outputTunedlogfreqspec].end(); ++it) {
        Feature f2 = *it; // logfreq spectrum
        Feature f3; // semitone spectrum
        Feature f4; // treble chromagram
        Feature f5; // bass chromagram
        Feature f6; // treble and bass chromagram
	    
        f3.hasTimestamp = true;
        f3.timestamp = f2.timestamp;
	        
        f4.hasTimestamp = true;
        f4.timestamp = f2.timestamp;
	        
        f5.hasTimestamp = true;
        f5.timestamp = f2.timestamp;
	        
        f6.hasTimestamp = true;
        f6.timestamp = f2.timestamp;
	        
        float b[nNote];
	
        bool some_b_greater_zero = false;
        float sumb = 0;
        for (int i = 0; i < nNote; i++) {
            // b[i] = m_dict[(nNote * count + i) % (nNote * 84)];
            b[i] = f2.values[i];
            sumb += b[i];
            if (b[i] > 0) {
                some_b_greater_zero = true;
            }            
        }
	    
        // here's where the non-negative least squares algorithm calculates the note activation x
	
        vector<float> chroma = vector<float>(12, 0);
        vector<float> basschroma = vector<float>(12, 0);
        float currval;
        int iSemitone = 0;
			
        if (some_b_greater_zero) {
            if (m_useNNLS == 0) {
                for (int iNote = nBPS/2 + 2; iNote < nNote - nBPS/2; iNote += nBPS) {
                    currval = 0;
                    for (int iBPS = -nBPS/2; iBPS < nBPS/2+1; ++iBPS) {
                        currval += b[iNote + iBPS] * (1-abs(iBPS*1.0/(nBPS/2+1)));						
                    }
                    f3.values.push_back(currval);
                    chroma[iSemitone % 12] += currval * treblewindow[iSemitone];
                    basschroma[iSemitone % 12] += currval * basswindow[iSemitone];
                    iSemitone++;
                }
		        
            } else {
                float x[84+1000];
                for (int i = 1; i < 1084; ++i) x[i] = 1.0;
                vector<int> signifIndex;
                int index=0;
                sumb /= 84.0;
                for (int iNote = nBPS/2 + 2; iNote < nNote - nBPS/2; iNote += nBPS) {
                    float currval = 0;
                    for (int iBPS = -nBPS/2; iBPS < nBPS/2+1; ++iBPS) {
                        currval += b[iNote + iBPS]; 
                    }
                    if (currval > 0) signifIndex.push_back(index);
                    f3.values.push_back(0); // fill the values, change later
                    index++;
                }
                float rnorm;
                float w[84+1000];
                float zz[84+1000];
                int indx[84+1000];
                int mode;
                int dictsize = nNote*signifIndex.size();
                // cerr << "dictsize is " << dictsize << "and values size" << f3.values.size()<< endl;
                float *curr_dict = new float[dictsize];
                for (int iNote = 0; iNote < (int)signifIndex.size(); ++iNote) {
                    for (int iBin = 0; iBin < nNote; iBin++) {
                        curr_dict[iNote * nNote + iBin] = 1.0 * m_dict[signifIndex[iNote] * nNote + iBin];
                    }
                }
                nnls(curr_dict, nNote, nNote, signifIndex.size(), b, x, &rnorm, w, zz, indx, &mode);
                delete [] curr_dict;
                for (int iNote = 0; iNote < (int)signifIndex.size(); ++iNote) {
                    f3.values[signifIndex[iNote]] = x[iNote];
                    // cerr << mode << endl;
                    chroma[signifIndex[iNote] % 12] += x[iNote] * treblewindow[signifIndex[iNote]];
                    basschroma[signifIndex[iNote] % 12] += x[iNote] * basswindow[signifIndex[iNote]];
                }
            }	
        } else {
            for (int i = 0; i < 84; ++i) f3.values.push_back(0);
        }
		

        f4.values = chroma; 
        f5.values = basschroma;
        chroma.insert(chroma.begin(), basschroma.begin(), basschroma.end()); // just stack the both chromas 
        f6.values = chroma; 
	        
        if (m_doNormalizeChroma > 0) {
            vector<float> chromanorm = vector<float>(3,0);			
            switch (int(m_doNormalizeChroma)) {
            case 0: // should never end up here
                break;
            case 1:
                chromanorm[0] = *max_element(f4.values.begin(), f4.values.end());
                chromanorm[1] = *max_element(f5.values.begin(), f5.values.end());
                chromanorm[2] = max(chromanorm[0], chromanorm[1]);
                break;
            case 2:
                for (vector<float>::iterator it = f4.values.begin(); it != f4.values.end(); ++it) {
                    chromanorm[0] += *it; 						
                }
                for (vector<float>::iterator it = f5.values.begin(); it != f5.values.end(); ++it) {
                    chromanorm[1] += *it; 						
                }
                for (vector<float>::iterator it = f6.values.begin(); it != f6.values.end(); ++it) {
                    chromanorm[2] += *it; 						
                }
                break;
            case 3:
                for (vector<float>::iterator it = f4.values.begin(); it != f4.values.end(); ++it) {
                    chromanorm[0] += pow(*it,2); 						
                }
                chromanorm[0] = sqrt(chromanorm[0]);
                for (vector<float>::iterator it = f5.values.begin(); it != f5.values.end(); ++it) {
                    chromanorm[1] += pow(*it,2); 						
                }
                chromanorm[1] = sqrt(chromanorm[1]);
                for (vector<float>::iterator it = f6.values.begin(); it != f6.values.end(); ++it) {
                    chromanorm[2] += pow(*it,2); 						
                }
                chromanorm[2] = sqrt(chromanorm[2]);
                break;
            }
            if (chromanorm[0] > 0) {
                for (int i = 0; i < (int)f4.values.size(); i++) {
                    f4.values[i] /= chromanorm[0];
                }
            }
            if (chromanorm[1] > 0) {
                for (int i = 0; i < (int)f5.values.size(); i++) {
                    f5.values[i] /= chromanorm[1];
                }
            }
            if (chromanorm[2] > 0) {
                for (int i = 0; i < (int)f6.values.size(); i++) {
                    f6.values[i] /= chromanorm[2];
                }
            }
        }
	
        fsOut[m_outputSemitonespectrum].push_back(f3);
        fsOut[m_outputChroma].push_back(f4);
        fsOut[m_outputBasschroma].push_back(f5);
        fsOut[m_outputBothchroma].push_back(f6);
        count++;
    }
    if (debug_on) cerr << "done." << endl;

    return fsOut;     

}

