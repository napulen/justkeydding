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
        // std::cout << it->getString() << ", " <<
        //     initialProbabilities[*it] << std::endl;
    }
    // Transition probabilities
    KeyTransition::KeyTransitionArray zero =
        KeyTransition("heatmap").getKeyTransition();
    KeyTransition::KeyTransitionArray rotated;
    std::map<Key, std::map<Key, double> > keyTransitions;
    for (Key::KeyVector::iterator it = keyVector.begin();
        it != keyVector.end(); it++) {
        int keyIndex = it->getInt();
        bool isMajorKey = keyIndex < 12;
        int tonicStart = isMajorKey ? 0 : 12;
        int tonicEnd = isMajorKey ? 12 : 24;
        int relativeStart = isMajorKey ? 12 : 0;
        int relativeEnd = isMajorKey ? 24 : 12;
        std::array<double, 12> tonic;
        std::array<double, 12> relative;
        std::copy(zero.begin() + tonicStart,
                zero.begin() + tonicEnd, tonic.begin());
        std::copy(zero.begin() + relativeStart,
                zero.begin() + relativeEnd, relative.begin());
        int tonicRotation = isMajorKey ?
                (12 - keyIndex) :
                ((24 - keyIndex + 9) % 12);
        int relativeRotation = (12 - (keyIndex % 12));
        std::rotate(tonic.begin(), tonic.begin() + tonicRotation, tonic.end());
        std::rotate(relative.begin(), relative.begin() + relativeRotation, relative.end());
        std::copy(tonic.begin(), tonic.end(), rotated.begin());
        std::copy(relative.begin(), relative.end(), rotated.begin() + 12);
        for (KeyTransition::KeyTransitionArray::iterator it = rotated.begin();
            it != rotated.end(); it++) {
            std::cout << (*it) << " ";
        }
        std::cout << tonicRotation << " "
                << relativeRotation << std::endl;
    }

    // TODO(napulen): Complete some actual unit tests
}
