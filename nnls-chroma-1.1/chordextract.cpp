/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
  NNLS-Chroma / Chordino

  Audio feature extraction plugins for chromagram and chord
  estimation.

  Centre for Digital Music, Queen Mary University of London.
  This file copyright 2014 QMUL.
    
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.  See the file
  COPYING included with this distribution for more information.
*/

/*
  Extract chords from an audio file, read using libsndfile.  Works by
  constructing the plugin as a C++ class directly, and using plugin
  adapters from the Vamp Host SDK to provide input.

  You can compile this with e.g. the following (Linux example):

  $ g++ -D_VAMP_PLUGIN_IN_HOST_NAMESPACE -O2 -ffast-math chordextract.cpp Chordino.cpp NNLSBase.cpp chromamethods.cpp viterbi.cpp nnls.c -o chordextract -lsndfile -lvamp-hostsdk -ldl

  But the same idea should work on any platform, so long as the Boost
  Tokenizer headers and the Vamp Host SDK library are available and
  the _VAMP_PLUGIN_IN_HOST_NAMESPACE preprocessor symbol is defined
  throughout.
*/

#define _VAMP_PLUGIN_IN_HOST_NAMESPACE 1

#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginBufferingAdapter.h>

#include "Chordino.h"

#include <sndfile.h>

#include <iostream>
#include <string>

using namespace std;
using namespace Vamp;
using namespace Vamp::HostExt;

int main(int argc, char **argv)
{
    const char *myname = argv[0];

    if (argc != 2) {
	cerr << "usage: " << myname << " file.wav" << endl;
	return 2;
    }

    const char *infile = argv[1];

    SF_INFO sfinfo;
    SNDFILE *sndfile = sf_open(infile, SFM_READ, &sfinfo);

    if (!sndfile) {
	cerr << myname << ": Failed to open input file " << infile
	     << ": " << sf_strerror(sndfile) << endl;
	return 1;
    }

    Chordino *chordino = new Chordino(sfinfo.samplerate);
    PluginInputDomainAdapter *ia = new PluginInputDomainAdapter(chordino);
    ia->setProcessTimestampMethod(PluginInputDomainAdapter::ShiftData);
    PluginBufferingAdapter *adapter = new PluginBufferingAdapter(ia);

    int blocksize = adapter->getPreferredBlockSize();

    // Plugin requires 1 channel (we will mix down)
    if (!adapter->initialise(1, blocksize, blocksize)) {
	cerr << myname << ": Failed to initialise Chordino adapter!" << endl;
	return 1;
    }

    float *filebuf = new float[sfinfo.channels * blocksize];
    float *mixbuf = new float[blocksize];

    Plugin::FeatureList chordFeatures;
    Plugin::FeatureSet fs;

    int chordFeatureNo = -1;
    Plugin::OutputList outputs = adapter->getOutputDescriptors();
    for (int i = 0; i < int(outputs.size()); ++i) {
	if (outputs[i].identifier == "simplechord") {
	    chordFeatureNo = i;
	}
    }
    if (chordFeatureNo < 0) {
	cerr << myname << ": Failed to identify chords output!" << endl;
	return 1;
    }
    
    int frame = 0;
    while (frame < sfinfo.frames) {

	int count = -1;
	if ((count = sf_readf_float(sndfile, filebuf, blocksize)) <= 0) break;

	// mix down
	for (int i = 0; i < blocksize; ++i) {
	    mixbuf[i] = 0.f;
	    if (i < count) {
		for (int c = 0; c < sfinfo.channels; ++c) {
		    mixbuf[i] += filebuf[i * sfinfo.channels + c] / sfinfo.channels;
		}
	    }
	}

	RealTime timestamp = RealTime::frame2RealTime(frame, sfinfo.samplerate);
	
	// feed to plugin: can just take address of buffer, as only one channel
	fs = adapter->process(&mixbuf, timestamp);

	chordFeatures.insert(chordFeatures.end(),
			     fs[chordFeatureNo].begin(),
			     fs[chordFeatureNo].end());

	frame += count;
    }

    sf_close(sndfile);

    // features at end of processing (actually Chordino does all its work here)
    fs = adapter->getRemainingFeatures();

    // chord output is output index 0
    chordFeatures.insert(chordFeatures.end(),
			 fs[chordFeatureNo].begin(),
			 fs[chordFeatureNo].end());

    for (int i = 0; i < (int)chordFeatures.size(); ++i) {
	cout << chordFeatures[i].timestamp.toString() << ": "
	     << chordFeatures[i].label << endl;
    }

    delete[] filebuf;
    delete[] mixbuf;
    
    delete adapter;
}

