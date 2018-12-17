/*
MIT License

Copyright (c) 2018 Nestor Napoles

MIDI parser that uses the MidiFile library by Craig Sapp

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

#ifndef INCLUDE_MIDI_H_
#define INCLUDE_MIDI_H_

#include <MidiFile.h>

#include "./pitchclass.h"
#include "./key.h"
#include "./keyprofile.h"
#include "./keytransition.h"
#include "./status.h"

#include<string>
#include<vector>
#include<map>
#include<cmath>
#include<iostream>
#include<fstream>
#include<sstream>

using std::cout;
using std::endl;
using std::ifstream;

namespace justkeydding {

class Midi {
 public:
  Midi(std::string fileName);
  PitchClass::PitchClassSequence getPitchClassSequence();
  int getStatus() const;
 private:
  int m_status;
  smf::MidiFile m_midifile;
};

}  // namespace justkeydding

#endif  // INCLUDE_MIDI_H_
