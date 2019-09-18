/*
MIT License

Copyright (c) 2018 Nestor Napoles

Methods for obtaining chromagrams either from a CSV or Audio file

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR sOTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef INCLUDE_CHROMAGRAM_H_
#define INCLUDE_CHROMAGRAM_H_

#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginBufferingAdapter.h>
#include <NNLSChroma.h>
#include <sndfile.h>

#include<string>
#include<vector>
#include<map>
#include<cmath>
#include<iostream>
#include<fstream>
#include<sstream>

#include "./pitchclass.h"
#include "./key.h"
#include "./keyprofile.h"
#include "./keytransition.h"
#include "./status.h"

using std::cout;
using std::endl;
using std::ifstream;

namespace justkeydding {

class Chromagram {
 public:
  typedef std::array<double,
      PitchClass::NUMBER_OF_PITCHCLASSES> ChromagramVector;
  typedef std::map<double, ChromagramVector> ChromagramMap;
  enum enFileType {
    FILETYPE_CSV,
    FILETYPE_AUDIO,
  };
  Chromagram(std::string fileName, enFileType fileType);
  PitchClass::PitchClassSequence getPitchClassSequence();
  void printChromagram();
  int getStatus() const;
  void printOriginalChromagram(bool);
 private:
  int m_status;
  ChromagramMap m_originalChromagramMap;
  ChromagramMap m_discreteChromagramMap;
  void getChromagramFromCsv(std::string csvFilename);
  void getChromagramFromAudio(std::string audioFilename);
  void discretizeChromagram();
};

}  // namespace justkeydding

#endif  // INCLUDE_CHROMAGRAM_H_
