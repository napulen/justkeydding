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

#include<map>
#include<vector>
#include<string>
#include<array>

#include"keyprofiles.h"

namespace justkeydding {

KeyProfile::KeyProfile() :
    KeyProfile("temperley", "sapp") {}

KeyProfile::KeyProfile(std::string keyProfile) :
    KeyProfile(keyProfile, keyProfile) {}

KeyProfile::KeyProfile(std::string majorKeyProfile,
        std::string minorKeyProfile) {
    // Initializes the stored key profiles
    init();
    // Can we find the ones sent to the constructor?
    if (m_majorKeyProfiles.find(majorKeyProfile) != m_majorKeyProfiles.end() &&
        m_minorKeyProfiles.find(minorKeyProfile) != m_minorKeyProfiles.end()) {
        m_majorKeyProfile = majorKeyProfile;
        m_minorKeyProfile = minorKeyProfile;
    }
    return;
}

int KeyProfile::init() {
    // Major Keys
    m_majorKeyProfiles["krumhansl_kessler"] = {
        6.35, 2.23, 3.48, 2.33, 4.38, 4.09,
        2.52, 5.19, 2.39, 3.66, 2.29, 2.88
    };
    m_majorKeyProfiles["aarden_essen"] = {
        17.7661, 0.145624, 14.9265, 0.160186, 19.8049, 11.3587,
        0.291248, 22.062, 0.145624, 8.15494, 0.232998, 4.95122
    };
    m_majorKeyProfiles["sapp"] = {
        2.0, 0.0, 1.0, 0.0, 1.0, 1.0,
        0.0, 2.0, 0.0, 1.0, 0.0, 1.0
    };
    m_majorKeyProfiles["bellman_budge"] = {
        16.8, 0.86, 12.95, 1.41, 13.49, 11.93,
        1.25, 20.28, 1.8, 8.04, 0.62, 10.57
    };
    m_majorKeyProfiles["temperley"] = {
        0.748, 0.06, 0.488, 0.082, 0.67, 0.46,
        0.096, 0.715, 0.104, 0.366, 0.057, 0.4,
    };
    // Minor Keys
    m_minorKeyProfiles["krumhansl_kessler"] = {
        6.33, 2.68, 3.52, 5.38, 2.6, 3.53,
        2.54, 4.75, 3.98, 2.69, 3.34, 3.17
    };
    m_minorKeyProfiles["aarden_essen"] = {
        18.2648, 0.737619, 14.0499, 16.8599, 0.702494, 14.4362,
        0.702494, 18.6161, 4.56621, 1.93186, 7.37619, 1.75623
    };
    m_minorKeyProfiles["sapp"] = {
        2.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 2.0, 1.0, 0.0, 0.5, 0.5
    };
    m_minorKeyProfiles["bellman_budge"] = {
        18.16, 0.69, 12.99, 13.34, 1.07, 11.15,
        1.38, 21.07, 7.49, 1.53, 0.92, 10.21
    };
    m_minorKeyProfiles["temperley"] = {
        0.712, 0.084, 0.474, 0.618, 0.049, 0.46,
        0.105, 0.747, 0.404, 0.067, 0.133, 0.33
    };
    return 0;
}

std::string KeyProfile::whichMajorKeyProfile() {
    return m_majorKeyProfile;
}
std::string KeyProfile::whichMinorKeyProfile() {
    return m_minorKeyProfile;
}
KeyProfile::KeyProfileArray KeyProfile::getMajorKeyProfile() {
    return m_majorKeyProfiles[m_majorKeyProfile];
}
KeyProfile::KeyProfileArray KeyProfile::getMinorKeyProfile() {
    return m_minorKeyProfiles[m_minorKeyProfile];
}

}  // namespace justkeydding
