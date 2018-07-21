/*
MIT License

Copyright (c) 2018 Nestor Napoles

Hidden Markov Model for Key Detection

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
#include<array>
#include<vector>
#include<iostream>
#include<algorithm>

#include "./pitchclass.h"
#include "./key.h"
#include "./keytransition.h"
#include "./keyprofile.h"
// #include "./hiddenmarkovmodel.h"

using justkeydding::PitchClass;
using justkeydding::KeyTransition;
using justkeydding::KeyProfile;
using justkeydding::Key;


int main(int argc, char *argv[]) {
    // Observation symbols section
    std::vector<PitchClass> pitchClassSequence;
    pitchClassSequence.push_back(PitchClass("C"));
    pitchClassSequence.push_back(PitchClass("E"));
    pitchClassSequence.push_back(PitchClass("G"));
    // States section
    Key::KeyVector keyVector =
        Key::getAllKeysVector();
    // Initial probabilities
    KeyTransition::KeyTransitionArray symmetrical =
        KeyTransition("symmetrical").getKeyTransition();
    std::map<Key, double> initialProbabilities;
    for (Key::KeyVector::iterator it = keyVector.begin();
        it != keyVector.end(); it++) {
        initialProbabilities[*it] = symmetrical[it->getInt()];
    }
    // Transition probabilities
    KeyTransition::KeyTransitionArray ktArray =
        KeyTransition("exponential10").getKeyTransition();
    KeyTransition::KeyTransitionArray rotated;
    std::map<Key, std::map<Key, double> > transitionProbabilities;
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
            transitionProbabilities[*itFromKey][*itToKey] =
                *(rotated.begin() + toKey);
            // std::cout
            //     << itFromKey->getString()
            //     << " -> " << itToKey->getString()
            //     << " = " << *(rotated.begin() + toKey)
            //     << std::endl;
        }
        // std::cout << std::endl;
    }
    // Emission probabilities
    PitchClass::PitchClassVector pitchClassVector =
        PitchClass::getAllPitchClassesVector();
    KeyProfile keyProfile("sapp");
    KeyProfile::KeyProfileArray keyProfileArray;
    std::map<Key, std::map<PitchClass, double> > emissionProbabilities;
    for (Key::KeyVector::const_iterator itKey = keyVector.begin();
             itKey != keyVector.end(); itKey++) {
        int key = itKey->getInt() % PitchClass::NUMBER_OF_PITCHCLASSES;
        int rotation = PitchClass::NUMBER_OF_PITCHCLASSES - key;
        keyProfileArray =
            itKey->isMajorKey() ?
            keyProfile.getMajorKeyProfile() :
            keyProfile.getMinorKeyProfile();
        std::rotate(
            keyProfileArray.begin(),
            keyProfileArray.begin() + rotation,
            keyProfileArray.end());
        for (PitchClass::PitchClassVector::const_iterator itPc =
                pitchClassVector.begin();
                itPc != pitchClassVector.end(); itPc++) {
            emissionProbabilities[*itKey][*itPc] =
                *(keyProfileArray.begin() + itPc->getInt());
            // std::cout
            //     << itKey->getString()
            //     << " -> " << itPc->getString()
            //     << " = " << *(keyProfileArray.begin() + itPc->getInt())
            //     << std::endl;
        }
    }
}
