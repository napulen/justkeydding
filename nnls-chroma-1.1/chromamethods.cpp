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

#include "chromamethods.h"

#include <cmath>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;


/** Special Convolution
    Special convolution is as long as the convolvee, i.e. the first argument. 
	In the "valid" core part of the convolution it contains the usual convolution 
	values, but the parts at the beginning (ending) that would normally be 
	calculated using zero padding simply have the same values as the first 
	(last) valid convolution bin.
**/

vector<float> SpecialConvolution(vector<float> convolvee, vector<float> kernel)
{
    float s;
    int m, n;
    int lenConvolvee = convolvee.size();
    int lenKernel = kernel.size();

    vector<float> Z(nNote,0);
    assert(lenKernel % 2 != 0); // no exception handling !!!
    
    for (n = lenKernel - 1; n < lenConvolvee; n++) {
    	s=0.0;
    	for (m = 0; m < lenKernel; m++) {
            // cerr << "m = " << m << ", n = " << n << ", n-m = " << (n-m) << '\n';
            s += convolvee[n-m] * kernel[m];
            // if (debug_on) cerr << "--> s = " << s << '\n';
    	}
        // cerr << n - lenKernel/2 << endl;
        Z[n -lenKernel/2] = s;
    }
    
    // fill upper and lower pads
    for (n = 0; n < lenKernel/2; n++) Z[n] = Z[lenKernel/2];    
    for (n = lenConvolvee; n < lenConvolvee +lenKernel/2; n++) Z[n - lenKernel/2] = 
                                                                   Z[lenConvolvee - lenKernel/2 -  1];
    return Z;
}

float cospuls(float x, float centre, float width) 
{
    float recipwidth = 1.0/width;
    if (abs(x - centre) <= 0.5 * width) {
        return cos((x-centre)*2*M_PI*recipwidth)*.5+.5;
    }
    return 0.0;
}

float pitchCospuls(float x, float centre, int binsperoctave) 
{
    float warpedf = -binsperoctave * (log2(centre) - log2(x));
    float out = cospuls(warpedf, 0.0, 2.0);
    // now scale to correct for note density
    float c = log(2.0)/binsperoctave;
    if (x > 0) {
        out = out / (c * x);
    } else {
        out = 0;
    }
    return out;
}

/**
 * Calculates a matrix that can be used to linearly map from the magnitude spectrum to a pitch-scale spectrum.
 * @return this always returns true, which is a bit stupid, really. The main purpose of the function is to change the values in the "matrix" pointed to by *outmatrix
 */
bool logFreqMatrix(int fs, int blocksize, float *outmatrix) {
	// TODO: rewrite so that everyone understands what is done here.
	// TODO: make this more general, such that it works with all minoctave, maxoctave and whatever nBPS (or check if it already does)
	
    int binspersemitone = nBPS; 
    int minoctave = 0; // this must be 0
    int maxoctave = 7; // this must be 7
    int oversampling = 80;
	
    // linear frequency vector
    vector<float> fft_f;
    for (int i = 0; i < blocksize/2; ++i) {
        fft_f.push_back(i * (fs * 1.0 / blocksize));
    }
    float fft_width = fs * 2.0 / blocksize;
	
    // linear oversampled frequency vector
    vector<float> oversampled_f;
    for (int i = 0; i < oversampling * blocksize/2; ++i) {
        oversampled_f.push_back(i * ((fs * 1.0 / blocksize) / oversampling));
    }
	
    // pitch-spaced frequency vector
    int minMIDI = 21 + minoctave * 12 - 1; // this includes one additional semitone!
    int maxMIDI = 21 + maxoctave * 12; // this includes one additional semitone!
    vector<float> cq_f;
    float oob = 1.0/binspersemitone; // one over binspersemitone
    for (int i = minMIDI; i < maxMIDI; ++i) {
        for (int k = 0; k < binspersemitone; ++k)	 {
            cq_f.push_back(440 * pow(2.0,0.083333333333 * (i+oob*k-69)));
        }
    }
    // cq_f.push_back(440 * pow(2.0,0.083333 * (minMIDI-oob-69)));
    cq_f.push_back(440 * pow(2.0,0.083333 * (maxMIDI-69)));

    int nFFT = fft_f.size();
	
    vector<float> fft_activation;
    for (int iOS = 0; iOS < 2 * oversampling; ++iOS) {
        float cosp = cospuls(oversampled_f[iOS],fft_f[1],fft_width);
        fft_activation.push_back(cosp);
        // cerr << cosp << endl;
    }

    for (int i = 0; i < nFFT * (int)cq_f.size(); ++i) {
        outmatrix[i] = 0.f;
    }
	
    float cq_activation;
    for (int iFFT = 1; iFFT < nFFT; ++iFFT) {
        // find frequency stretch where the oversampled vector can be non-zero (i.e. in a window of width fft_width around the current frequency)
        int curr_start = oversampling * iFFT - oversampling;
        int curr_end = oversampling * iFFT + oversampling; // don't know if I should add "+1" here
        // cerr << oversampled_f[curr_start] << " " << fft_f[iFFT] << " " << oversampled_f[curr_end] << endl;
        for (int iCQ = 0; iCQ < (int)cq_f.size(); ++iCQ) {
            if (cq_f[iCQ] * pow(2.0, 0.084) + fft_width > fft_f[iFFT] && cq_f[iCQ] * pow(2.0, -0.084 * 2) - fft_width < fft_f[iFFT]) { // within a generous neighbourhood
                for (int iOS = curr_start; iOS < curr_end; ++iOS) {
                    cq_activation = pitchCospuls(oversampled_f[iOS],cq_f[iCQ],binspersemitone*12);
                    // cerr << oversampled_f[iOS] << " " << cq_f[iCQ] << " " << cq_activation << endl;
                    outmatrix[iFFT + nFFT * iCQ] += cq_activation * fft_activation[iOS-curr_start];
                }				
             }
        }
    }
    return true;	
}

void dictionaryMatrix(float* dm, float s_param) {
	// TODO: make this more general, such that it works with all minoctave, maxoctave and even more than one note per semitone
    int binspersemitone = nBPS;
    int minoctave = 0; // this must be 0
    int maxoctave = 7; // this must be 7
	
    // pitch-spaced frequency vector
    int minMIDI = 21 + minoctave * 12 - 1; // this includes one additional semitone!
    int maxMIDI = 21 + maxoctave * 12; // this includes one additional semitone!
    vector<float> cq_f;
    float oob = 1.0/binspersemitone; // one over binspersemitone
    for (int i = minMIDI; i < maxMIDI; ++i) {
        for (int k = 0; k < binspersemitone; ++k)	 {
            cq_f.push_back(440 * pow(2.0,0.083333333333 * (i+oob*k-69)));
        }
    }
    cq_f.push_back(440 * pow(2.0,0.083333 * (maxMIDI-69)));

//    float curr_f;
    float floatbin;
    float curr_amp;
    // now for every combination calculate the matrix element
    for (int iOut = 0; iOut < 12 * (maxoctave - minoctave); ++iOut) {
        // cerr << iOut << endl;
        for (int iHarm = 1; iHarm <= 20; ++iHarm) {
//            curr_f = 440 * pow(2,(minMIDI-69+iOut)*1.0/12) * iHarm;
            // if (curr_f > cq_f[nNote-1])  break;
            floatbin = ((iOut + 1) * binspersemitone + 1) + binspersemitone * 12 * log2(iHarm);
            // cerr << floatbin << endl;
            curr_amp = pow(s_param,float(iHarm-1));
            // cerr << "curramp" << curr_amp << endl;
            for (int iNote = 0; iNote < nNote; ++iNote) {
                if (abs(iNote+1.0-floatbin)<2) {
                    dm[iNote  + nNote * iOut] += cospuls(iNote+1.0, floatbin, binspersemitone + 0.0) * curr_amp;
                    // dm[iNote + nNote * iOut] += 1 * curr_amp;
                }
            }
        }
    }
}

static
std::vector<std::string>
getPluginPath()
{
    //!!! This is duplicated from PluginHostAdapter::getPluginPath,
    //!!! which is not available to us in the plugin (only to the
    //!!! host)

    std::vector<std::string> path;
    std::string envPath;

    char *cpath = getenv("VAMP_PATH");
    if (cpath) envPath = cpath;

#ifdef _WIN32
#define PATH_SEPARATOR ';'
#define DEFAULT_VAMP_PATH "%ProgramFiles%\\Vamp Plugins"
#else
#define PATH_SEPARATOR ':'
#ifdef __APPLE__
#define DEFAULT_VAMP_PATH "$HOME/Library/Audio/Plug-Ins/Vamp:/Library/Audio/Plug-Ins/Vamp"
#else
#define DEFAULT_VAMP_PATH "$HOME/vamp:$HOME/.vamp:/usr/local/lib/vamp:/usr/lib/vamp"
#endif
#endif

    if (envPath == "") {
        envPath = DEFAULT_VAMP_PATH;
        char *chome = getenv("HOME");
        if (chome) {
            std::string home(chome);
            std::string::size_type f;
            while ((f = envPath.find("$HOME")) != std::string::npos &&
                    f < envPath.length()) {
                envPath.replace(f, 5, home);
            }
        }
#ifdef _WIN32
        char *cpfiles = getenv("ProgramFiles");
        if (!cpfiles) cpfiles = (char *)"C:\\Program Files";
        std::string pfiles(cpfiles);
        std::string::size_type f;
        while ((f = envPath.find("%ProgramFiles%")) != std::string::npos &&
               f < envPath.length()) {
            envPath.replace(f, 14, pfiles);
        }
#endif
    }

    std::string::size_type index = 0, newindex = 0;

    while ((newindex = envPath.find(PATH_SEPARATOR, index)) < envPath.size()) {
	path.push_back(envPath.substr(index, newindex - index));
	index = newindex + 1;
    }
    
    path.push_back(envPath.substr(index));

    return path;
}



static vector<string> staticChordnames() {
    vector<string> chordnames;
    chordnames.push_back("");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("");// =0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("m");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0
    chordnames.push_back("m");//=0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0
    chordnames.push_back("m7b5");//=0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0
    chordnames.push_back("m7b5");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0
    chordnames.push_back("6");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0
    chordnames.push_back("7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0
    chordnames.push_back("maj7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1
    chordnames.push_back("m7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0
    chordnames.push_back("m6");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0
    chordnames.push_back("");//=0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("");//=0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("dim");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0
    chordnames.push_back("aug");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0
    chordnames.push_back("");//=0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("");//=0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0 
    chordnames.push_back("7");//=0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0
	// from here: Harte syntax
    chordnames.push_back("");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("");// =0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back(":min");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0
    chordnames.push_back(":min");//=0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0
    chordnames.push_back(":hdim7");//=0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0
    chordnames.push_back(":hdim7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0
    chordnames.push_back(":maj6");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0
    chordnames.push_back(":7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0
    chordnames.push_back(":maj7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1
    chordnames.push_back(":min7");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0
    chordnames.push_back(":min6");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0
    chordnames.push_back("");//=0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("");//=0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back(":dim");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0
    chordnames.push_back(":aug");//=1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0
    chordnames.push_back("");//=0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0
    chordnames.push_back("");//=0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0 
    chordnames.push_back(":7");//=0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0
    return chordnames;
}

static vector<float> staticChordvalues() {
    vector<float> chordvalues;
    float chordvaluearray[] = {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0};
    for (int iChord = 0; iChord < 18; ++iChord) {
        for (int iNote = 0; iNote < 24; iNote++) {
            chordvalues.push_back(chordvaluearray[24*iChord+iNote]);
        }
    }
    return chordvalues;
}

vector<string> chordDictionary(vector<float> *mchorddict, vector<vector<int> > *m_chordnotes, float boostN, float harte_syntax) {    
    
    typedef tokenizer<char_separator<char> > Tok;
    char_separator<char> sep(",; ","=");

    string chordDictBase("chord.dict");
    string chordDictFilename;

    vector<string> ppath = getPluginPath();
    
	const char* notenames[24] = {
	    "A  (bass)","Bb (bass)","B  (bass)","C  (bass)","C# (bass)","D  (bass)","Eb (bass)","E  (bass)","F  (bass)","F# (bass)","G  (bass)","Ab (bass)",
	    "A","Bb","B","C","C#","D","Eb","E","F","F#","G","Ab"};

	const char* bassnames[13][12] ={
	    {"A","","B","C","C#","D","","E","","F#","G","G#"},
	    {"Bb","","C","Db","D","Eb","","F","","G","Ab","A"},
	    {"B","","C#","D","D#","E","","F#","","G#","A","A#"},
	    {"C","","D","Eb","E","F","","G","","A","Bb","B"},
	    {"C#","","D#","E","E#","F#","","G#","","A#","B","B#"},
	    {"D","","E","F","F#","G","","A","","B","C","C#"},
	    {"Eb","","F","Gb","G","Ab","","Bb","","C","Db","D"},
	    {"E","","F#","G","G#","A","","B","","C#","D","D#"},
	    {"F","","G","Ab","A","Bb","","C","","D","Eb","E"},
	    {"F#","","G#","A","A#","B","","C#","","D#","E","E#"},
	    {"G","","A","Bb","B","C","","D","","E","F","F#"},
	    {"Ab","","Bb","Cb","C","Db","","Eb","","F","Gb","G"},
            {"1","","2","b3","3","4","","5","","6","b7","7"}
	};

    bool hasExternalDictinoary = true;
	int ppathsize = static_cast<int>(ppath.size());
    for (int i = 0; i < ppathsize; ++i) {
    	chordDictFilename = ppath[i] + "/" + chordDictBase;
//    	cerr << "Looking for chord.dict in " << chordDictFilename << "..." ;
    	fstream fin;
        fin.open(chordDictFilename.c_str(),ios::in);
        if( fin.is_open() )
        {
            fin.close();
//    	    cerr << " success." << endl;
    	    break;
        } else {
            if (i+1 < ppathsize) {
//                cerr << " (not found yet) ..." << endl;
            } else {
//                cerr << "* WARNING: failed to find chord dictionary, using default chord dictionary." << endl;
                hasExternalDictinoary = false;                
            } 
        }
    }

    iostreams::stream<iostreams::file_source> chordDictFile(chordDictFilename);
    string line;
    // int iElement = 0;
    int nChord = 0;
	
	vector<float> tempChordDict = staticChordvalues();
    vector<string> tempChordNames = staticChordnames();
	if (harte_syntax == 1.0) {
		tempChordNames.erase(tempChordNames.begin(),tempChordNames.begin()+tempChordNames.size()/2);
	} else {
		tempChordNames.erase(tempChordNames.begin()+tempChordNames.size()/2,tempChordNames.begin()+tempChordNames.size());
	}

    vector<string> loadedChordNames;
    vector<float> loadedChordDict;
    if (hasExternalDictinoary && chordDictFile.is_open()) {
        tempChordDict.clear();
        tempChordNames.clear();
        while (std::getline(chordDictFile, line)) { // loop over lines in chord.dict file	            	
            // first, get the chord definition
            string chordType;
            vector<float> tempPCVector;			
            // cerr << line << endl;
            if (!line.empty() && line.substr(0,1) != "#") {
                Tok tok(line, sep);			
                for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter) { // loop over line elements
                    string tempString = *tok_iter;
                    // cerr << tempString << endl;
                    if (tok_iter == tok.begin()) { // either the chord name or a colon                        
                        if (tempString == "=") {
                            chordType = "";
                        } else {
                            chordType = tempString;
                            tok_iter++;                            
                        }
                    } else {
                        tempChordDict.push_back(lexical_cast<float>(*tok_iter));
                    }
                }                
                tempChordNames.push_back(chordType);
            }
        }
        cerr << "-----------------> " << tempChordNames.size() << endl;
    }
    
        
    for (int iType = 0; iType < (int)tempChordNames.size(); ++iType) {
        // now make all 12 chords of every type
        for (int iSemitone = 0; iSemitone < 12; iSemitone++) {	
            vector<int> tempchordnotes;			
            // add bass slash notation
            string slashNotation = "";
            for (int kSemitone = 1; kSemitone < 12; kSemitone++) {
                if (tempChordDict[24*iType+(kSemitone) % 12] > 0.99) {
					if (harte_syntax == 0.0) {
						slashNotation = bassnames[iSemitone][kSemitone];
					} else {
					        slashNotation = bassnames[12][kSemitone]; 
                                        }
                }
            }
            if (slashNotation=="") tempchordnotes.push_back(MIDI_basenote + (iSemitone+12) % 12);
            for (int kSemitone = 0; kSemitone < 12; kSemitone++) { // bass pitch classes
                // cerr << ((kSemitone - iSemitone + 12) % 12) << endl;
                float bassValue = 0;
                if (tempChordDict[24*iType+(kSemitone - iSemitone + 12) % 12]==1) {
                    bassValue = 1;
                    tempchordnotes.push_back(MIDI_basenote + (kSemitone+12) % 12);
                } else {
                    if (tempChordDict[24*iType+((kSemitone - iSemitone + 12) % 12) + 12] == 1) bassValue = 0.5;
                }
                loadedChordDict.push_back(bassValue);                        
            }
            for (int kSemitone = 0; kSemitone < 12; kSemitone++) { // chord pitch classes
                loadedChordDict.push_back(tempChordDict[24*iType+((kSemitone - iSemitone + 12) % 12) + 12]);
                if (tempChordDict[24*iType+((kSemitone - iSemitone + 12) % 12) + 12] > 0) tempchordnotes.push_back(MIDI_basenote + (kSemitone+12+6) % 12 - 6 + 24);
            }
            ostringstream os;				
            if (slashNotation.empty()) {
                os << notenames[12+iSemitone] << tempChordNames[iType];
            } else {
                os << notenames[12+iSemitone] << tempChordNames[iType] << "/" << slashNotation;
            }
            // cerr << os.str() << endl;
            loadedChordNames.push_back(os.str());                
            
            m_chordnotes->push_back(tempchordnotes);
            // for (int iNote = 0; iNote < tempchordnotes.size(); ++iNote) {
            //     cerr << tempchordnotes[iNote] << " ";
            // }
            // cerr << endl;
        }
    }
    

    // N type
    loadedChordNames.push_back("N");
    for (int kSemitone = 0; kSemitone < 12; kSemitone++) loadedChordDict.push_back(0.5);
    for (int kSemitone = 0; kSemitone < 12; kSemitone++) loadedChordDict.push_back(1.0);
    vector<int> tempchordvector;
    m_chordnotes->push_back(tempchordvector);
    float exponent = 2.0;
    // float m_boostN = 1.1;
    // cerr << " N BOOST : " << boostN << endl << endl;
    for (int iChord = 0; iChord < (int)loadedChordDict.size()/24; iChord++) {
        float sum = 0;
        float stand = 0;
        for (int iST = 0; iST < 24; ++iST) {
            sum += loadedChordDict[24 * iChord + iST];
        }
        for (int iST = 0; iST < 24; ++iST) {
            // loadedChordDict[24 * iChord + iST] -= sum/24;
            stand += pow(abs(loadedChordDict[24 * iChord + iST]),exponent)/24;
        }
        if (iChord < (int)loadedChordDict.size()/24 - 1) {
            stand = powf(stand,1.0f/exponent);
        } else {
            stand = powf(stand,1.0f/exponent) / (1+boostN);
        }
        for (int iST = 0; iST < 24; ++iST) {
            loadedChordDict[24 * iChord + iST] /= stand;            
        }
        
    }
    


    nChord = 0;
    for (int i = 0; i < (int)loadedChordNames.size(); i++) {
        nChord++;
    }
    chordDictFile.close();


    // mchorddict = new float[nChord*24];
    for (int i = 0; i < nChord*24; i++) {
        mchorddict->push_back(loadedChordDict[i]);			
    }
	
    // cerr << "before leaving" << chordnames[1] << endl;
    return loadedChordNames;
}

    
