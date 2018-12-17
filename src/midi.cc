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


#include "./midi.h"

namespace justkeydding {

Midi::Midi(std::string fileName) {
   smf::Midifile
}

void Midi::getPitchClassSequence(smf::MidiFile& midifile) {
   midifile.absoluteTicks();
   midifile.joinTracks();
   int key = 0;
   for (int i=0; i < midifile.getNumEvents(0); i++) {
      int command = midifile[0][i][0] & 0xf0;
      // noteOn event with velocity > 0
      if (command == 0x90 && midifile[0][i][2] != 0) {
         pitch = midifile[0][i][1];
         std::cout << key % 12 << " ";
      }
   }
}

int main(int argc, char* argv[]) {   
   smf::MidiFile midifile(argv[1]);
   parseMidiNoteOnEvents(midifile);
   return 0;
}

} // namespace justkeydding