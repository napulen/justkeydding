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

#ifndef INCLUDE_PITCHCLASS_H_
#define INCLUDE_PITCHCLASS_H_

#include<map>
#include<vector>
#include<string>
#include<array>
#include<algorithm>
#include<iostream>

namespace justkeydding {

class PitchClass {
 public:
    typedef std::vector<PitchClass> PitchClassVector;
    typedef std::vector<PitchClass> PitchClassSequence;
    explicit PitchClass(std::string pitchClass);
    explicit PitchClass(int pitchClass);
    int getInt() const;
    std::string getString() const;
    static PitchClassVector getAllPitchClassesVector();
    bool operator==(const PitchClass &pitchClass) const;
    bool operator!=(const PitchClass &pitchClass) const;
    bool operator<(const PitchClass &pitchClass) const;
    enum enPitchClass {
        PITCHCLASS_B_SHARP, PITCHCLASS_C_NATURAL = 0,
        PITCHCLASS_C_SHARP, PITCHCLASS_D_FLAT = 1,
        PITCHCLASS_D_NATURAL,
        PITCHCLASS_D_SHARP, PITCHCLASS_E_FLAT = 3,
        PITCHCLASS_E_NATURAL, PITCHCLASS_F_FLAT = 4,
        PITCHCLASS_E_SHARP, PITCHCLASS_F_NATURAL = 5,
        PITCHCLASS_F_SHARP, PITCHCLASS_G_FLAT = 6,
        PITCHCLASS_G_NATURAL,
        PITCHCLASS_G_SHARP, PITCHCLASS_A_FLAT = 8,
        PITCHCLASS_A_NATURAL,
        PITCHCLASS_A_SHARP, PITCHCLASS_B_FLAT = 10,
        PITCHCLASS_B_NATURAL, PITCHCLASS_C_FLAT = 11,
        NUMBER_OF_PITCHCLASSES
    };

 private:
    int m_pitchClass;
    std::string m_pitchClassString;
    std::map<std::string, int> m_stringToIntMap;
    const std::string m_allPitchClassStrings[
        NUMBER_OF_PITCHCLASSES] = {
        "c", "c#", "d", "eb", "e", "f",
        "f#", "g", "ab", "a", "bb", "b"
    };
    void initStringToIntMap();
};

}  // namespace justkeydding

#endif  // INCLUDE_PITCHCLASS_H_
