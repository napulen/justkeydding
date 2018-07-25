/*
MIT License

Copyright (c) 2018 Nestor Napoles

Base class for pitch-class and key definitions

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

#include "./pitchclass.h"

namespace justkeydding {

PitchClass::PitchClass(std::string pitchClass) {
    initStringToIntMap();
    std::transform(
        pitchClass.begin(), pitchClass.end(),
        pitchClass.begin(), ::tolower);
    if (m_stringToIntMap.find(pitchClass) != m_stringToIntMap.end()) {
        m_pitchClass = m_stringToIntMap.at(pitchClass);
        m_pitchClassString = m_allPitchClassStrings[m_pitchClass];
    }
}

PitchClass::PitchClass(int pitchClass) {
    initStringToIntMap();
    if (pitchClass >= 0 && pitchClass < NUMBER_OF_PITCHCLASSES) {
        m_pitchClass = pitchClass;
        m_pitchClassString = m_allPitchClassStrings[m_pitchClass];
    }
}

PitchClass::PitchClassVector PitchClass::getAllPitchClassesVector() {
    PitchClassVector pitchClassVector;
    for (int pitchClassIndex = 0;
            pitchClassIndex < NUMBER_OF_PITCHCLASSES; pitchClassIndex++) {
         pitchClassVector.push_back(PitchClass(pitchClassIndex));
    }
    return pitchClassVector;
}

void PitchClass::initStringToIntMap() {
    m_stringToIntMap["b#"] = PITCHCLASS_B_SHARP;
    m_stringToIntMap["c"] = PITCHCLASS_C_NATURAL;
    m_stringToIntMap["c#"] = PITCHCLASS_C_SHARP;
    m_stringToIntMap["db"] = PITCHCLASS_D_FLAT;
    m_stringToIntMap["d"] = PITCHCLASS_D_NATURAL;
    m_stringToIntMap["d#"] = PITCHCLASS_D_SHARP;
    m_stringToIntMap["eb"] = PITCHCLASS_E_FLAT;
    m_stringToIntMap["e"] = PITCHCLASS_E_NATURAL;
    m_stringToIntMap["fb"] = PITCHCLASS_F_FLAT;
    m_stringToIntMap["e#"] = PITCHCLASS_E_SHARP;
    m_stringToIntMap["f"] = PITCHCLASS_F_NATURAL;
    m_stringToIntMap["f#"] = PITCHCLASS_F_SHARP;
    m_stringToIntMap["gb"] = PITCHCLASS_G_FLAT;
    m_stringToIntMap["g"] = PITCHCLASS_G_NATURAL;
    m_stringToIntMap["g#"] = PITCHCLASS_G_SHARP;
    m_stringToIntMap["ab"] = PITCHCLASS_A_FLAT;
    m_stringToIntMap["a"] = PITCHCLASS_A_NATURAL;
    m_stringToIntMap["a#"] = PITCHCLASS_A_SHARP;
    m_stringToIntMap["bb"] = PITCHCLASS_B_FLAT;
    m_stringToIntMap["b"] = PITCHCLASS_B_NATURAL;
    m_stringToIntMap["cb"] = PITCHCLASS_C_FLAT;
}

int PitchClass::getInt() const {
    return m_pitchClass;
}

std::string PitchClass::getString() const {
    return m_pitchClassString;
}

bool PitchClass::operator==(const PitchClass &pitchClass) const {
    return m_pitchClass == pitchClass.getInt();
}

bool PitchClass::operator!=(const PitchClass &pitchClass) const {
    return m_pitchClass != pitchClass.getInt();
}

bool PitchClass::operator<(const PitchClass &pitchClass) const {
    return m_pitchClass < pitchClass.getInt();
}

}  // namespace justkeydding
