/*
MIT License

Copyright (c) 2018 Nestor Napoles

Common key-profiles for key detection algorithms

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

#ifndef INCLUDE_KEYPROFILE_H_
#define INCLUDE_KEYPROFILE_H_

#include<map>
#include<string>
#include<array>
#include<algorithm>

#include "./pitchclass.h"
#include "./key.h"

namespace justkeydding {

class KeyProfile {
 public:
    typedef std::array<double,
        PitchClass::NUMBER_OF_PITCHCLASSES> KeyProfileArray;
    typedef std::map<Key, std::map<PitchClass, double> > KeyProfileMap;
    KeyProfile();
    explicit KeyProfile(std::string keyProfile);
    explicit KeyProfile(std::string majKeyProfile, std::string minKeyProfile);
    explicit KeyProfile(
        std::string majKeyProfile,
        std::string minKeyProfile,
        KeyProfileArray majCustom, 
        KeyProfileArray minCustom);
    bool isValidMajorKeyProfile(std::string keyProfile);
    bool isValidMinorKeyProfile(std::string keyProfile);
    std::string whichMajorKeyProfile();
    std::string whichMinorKeyProfile();
    KeyProfileArray getMajorKeyProfile();
    KeyProfileArray getMinorKeyProfile();
    KeyProfileMap getKeyProfileMap();

 private:    
    std::string m_majorKeyProfile;
    std::string m_minorKeyProfile;
    std::map<std::string, KeyProfileArray> m_majorKeyProfiles;
    std::map<std::string, KeyProfileArray> m_minorKeyProfiles;
    void initKeyProfileArrays();
};

}  // namespace justkeydding

#endif  // INCLUDE_KEYPROFILE_H_
