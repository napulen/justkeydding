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
#include "./hiddenmarkovmodel.h"

using justkeydding::PitchClass;
using justkeydding::KeyTransition;
using justkeydding::KeyProfile;
using justkeydding::Key;
using justkeydding::HiddenMarkovModel;


int main(int argc, char *argv[]) {
    // Observation symbols section
    std::vector<PitchClass> pitchClassSequence;
    pitchClassSequence.push_back(PitchClass("C"));
    pitchClassSequence.push_back(PitchClass("D"));
    pitchClassSequence.push_back(PitchClass("E"));
    pitchClassSequence.push_back(PitchClass("F"));
    pitchClassSequence.push_back(PitchClass("G"));
    pitchClassSequence.push_back(PitchClass("A"));
    pitchClassSequence.push_back(PitchClass("B"));
    pitchClassSequence.push_back(PitchClass("A"));
    pitchClassSequence.push_back(PitchClass("G"));
    pitchClassSequence.push_back(PitchClass("F#"));
    pitchClassSequence.push_back(PitchClass("E"));
    pitchClassSequence.push_back(PitchClass("D"));
    pitchClassSequence.push_back(PitchClass("C#"));
    pitchClassSequence.push_back(PitchClass("D"));
    pitchClassSequence.push_back(PitchClass("E"));
    pitchClassSequence.push_back(PitchClass("F#"));
    pitchClassSequence.push_back(PitchClass("G"));
    pitchClassSequence.push_back(PitchClass("A"));
    pitchClassSequence.push_back(PitchClass("B"));
    pitchClassSequence.push_back(PitchClass("C"));
    pitchClassSequence.push_back(PitchClass("B"));
    pitchClassSequence.push_back(PitchClass("A"));
    pitchClassSequence.push_back(PitchClass("G"));
    pitchClassSequence.push_back(PitchClass("F"));
    pitchClassSequence.push_back(PitchClass("E"));
    pitchClassSequence.push_back(PitchClass("D"));
    pitchClassSequence.push_back(PitchClass("C"));
    // States section
    Key::KeyVector keyVector =
        Key::getAllKeysVector();
    // Initial probabilities
    KeyTransition::KeyTransitionArray symmetrical =
        KeyTransition("symmetrical").getKeyTransitionArray();
    std::map<Key, double> initialProbabilities;
    for (Key::KeyVector::iterator it = keyVector.begin();
        it != keyVector.end(); it++) {
        initialProbabilities[*it] = symmetrical[it->getInt()];
    }
    // Transition probabilities
    KeyTransition::KeyTransitionMap transitionProbabilities =
        KeyTransition("exponential10").getKeyTransitionMap();
    // Emission probabilities
    KeyProfile::KeyProfileMap emissionProbabilities =
        KeyProfile("temperley", "sapp").getKeyProfileMap();
    // HiddenMarkovModel stuff
    HiddenMarkovModel hmm(
        pitchClassSequence,
        keyVector,
        initialProbabilities,
        transitionProbabilities,
        emissionProbabilities);
    hmm.printOutput();
    hmm.runViterbi();
}
