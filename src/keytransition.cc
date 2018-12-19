/*
MIT License

Copyright (c) 2018 Nestor Napoles

Key transition probability distributions for the hidden markov model

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

#include "./keytransition.h"

namespace justkeydding {

KeyTransition::KeyTransition() :
    KeyTransition("exponential10") {}

KeyTransition::KeyTransition(std::string keyTransition) :
    KeyTransition(
        keyTransition,
        {0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0}) {}


KeyTransition::KeyTransition(std::string keyTransition,
        KeyTransitionArray customKeyTransition) {
    initKeyTransitionArrays();
    if (keyTransition == "custom") {
        m_keyTransitions[keyTransition] = customKeyTransition;
    }
    if (isValidKeyTransition(keyTransition)) {
        m_keyTransition = keyTransition;
    }
}

void KeyTransition::initKeyTransitionArrays() {
    m_keyTransitions["linear"] = {
        9.0/132, 4.0/132, 6.0/132, 6.0/132, 5.0/132, 8.0/132,
        1.0/132, 8.0/132, 5.0/132, 6.0/132, 6.0/132, 4.0/132,
        8.0/132, 2.0/132, 7.0/132, 3.0/132, 7.0/132, 7.0/132,
        3.0/132, 7.0/132, 2.0/132, 8.0/132, 5.0/132, 5.0/132,
    };
    m_keyTransitions["exponential2"] = {
        256.0/1245, 8.0/1245, 32.0/1245, 32.0/1245, 16.0/1245, 128.0/1245,
        1.0/1245, 128.0/1245, 16.0/1245, 32.0/1245, 32.0/1245, 8.0/1245,
        128.0/1245, 2.0/1245, 64.0/1245, 4.0/1245, 64.0/1245, 64.0/1245,
        4.0/1245, 64.0/1245, 2.0/1245, 128.0/1245, 16.0/1245, 16.0/1245,
    };
    m_keyTransitions["exponential10"] = {
        100000000.0/144442221, 1000.0/144442221, 100000.0/144442221,
        100000.0/144442221, 10000.0/144442221, 10000000.0/144442221,
        1.0/144442221, 10000000.0/144442221, 10000.0/144442221,
        100000.0/144442221, 100000.0/144442221, 1000.0/144442221,
        10000000.0/144442221, 10.0/144442221, 1000000.0/144442221,
        100.0/144442221, 1000000.0/144442221, 1000000.0/144442221,
        100.0/144442221, 1000000.0/144442221, 10.0/144442221,
        10000000.0/144442221, 10000.0/144442221, 10000.0/144442221,
    };
    m_keyTransitions["zero"] = {
        1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    };
    m_keyTransitions["symmetrical"] = {
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
    };
    m_keyTransitions["heatmap"] = {
        1.0, 6.0, 4.0, 4.0, 5.0, 2.0, 9.0, 2.0, 5.0, 4.0, 4.0, 6.0,
        2.0, 8.0, 3.0, 7.0, 3.0, 3.0, 7.0, 3.0, 8.0, 2.0, 5.0, 5.0,
    };
}

bool KeyTransition::isValidKeyTransition(std::string keyTransition) {
    return m_keyTransitions.find(keyTransition) != m_keyTransitions.end();
}

std::string KeyTransition::whichKeyTransition() {
    return m_keyTransition;
}

KeyTransition::KeyTransitionArray KeyTransition::getKeyTransitionArray() {
    return m_keyTransitions[m_keyTransition];
}

KeyTransition::KeyTransitionMap KeyTransition::getKeyTransitionMap() {
    KeyTransition::KeyTransitionArray ktArray = getKeyTransitionArray();
    Key::KeyVector keyVector = Key::getAllKeysVector();
    KeyTransitionMap keyTransitionMap;
    KeyTransition::KeyTransitionArray rotated;
    for (Key::KeyVector::const_iterator itFromKey = keyVector.begin();
            itFromKey != keyVector.end(); itFromKey++) {
        int fromKey = itFromKey->getInt() % PitchClass::NUMBER_OF_PITCHCLASSES;
        int keyRotation = PitchClass::NUMBER_OF_PITCHCLASSES - fromKey;
        std::array<double, PitchClass::NUMBER_OF_PITCHCLASSES> tonic;
        std::array<double, PitchClass::NUMBER_OF_PITCHCLASSES> relative;
        if (itFromKey->isMajorKey()) {
            std::copy(
                ktArray.begin() + Key::FIRST_MAJOR_KEY,
                ktArray.begin() + Key::LAST_MAJOR_KEY + 1,
                tonic.begin());
            std::copy(
                ktArray.begin() + Key::FIRST_MINOR_KEY,
                ktArray.begin() + Key::LAST_MINOR_KEY + 1,
                relative.begin());
        } else if (itFromKey->isMinorKey()) {
            std::rotate_copy(
                ktArray.begin() + Key::FIRST_MINOR_KEY,
                ktArray.begin()
                    + Key::FIRST_MINOR_KEY
                    + PitchClass::PITCHCLASS_A_NATURAL,
                ktArray.begin() + Key::LAST_MINOR_KEY + 1,
                tonic.begin());
            std::copy(
                ktArray.begin() + Key::FIRST_MAJOR_KEY,
                ktArray.begin() + Key::LAST_MAJOR_KEY + 1,
                relative.begin());
        }
        std::rotate(
            tonic.begin(),
            tonic.begin() + keyRotation,
            tonic.end());
        std::rotate(
            relative.begin(),
            relative.begin() + keyRotation,
            relative.end());
        std::copy(
            tonic.begin(),
            tonic.end(),
            rotated.begin() + Key::FIRST_MAJOR_KEY);
        std::copy(
            relative.begin(),
            relative.end(),
            rotated.begin() + Key::FIRST_MINOR_KEY);
        for (Key::KeyVector::const_iterator itToKey = keyVector.begin();
                itToKey != keyVector.end(); itToKey++) {
            int toKey = itToKey->getInt();
            keyTransitionMap[*itFromKey][*itToKey] =
                *(rotated.begin() + toKey);
            // std::cout
            //     << itFromKey->getString()
            //     << " -> " << itToKey->getString()
            //     << " = " << *(rotated.begin() + toKey)
            //     << std::endl;
        }
        // std::cout << std::endl;
    }
    return keyTransitionMap;
}

}  // namespace justkeydding
