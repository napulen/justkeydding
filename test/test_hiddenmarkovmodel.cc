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
    ////////////////////////////
    // First Hidden Markov Model
    ////////////////////////////
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
    Key::KeySequence keySequence;
    HiddenMarkovModel hmm(
        pitchClassSequence,
        keyVector,
        initialProbabilities,
        transitionProbabilities,
        emissionProbabilities);
    hmm.runViterbi();
    keySequence = hmm.getKeySequence();
    /////////////////////////////
    // Second Hidden Markov Model
    /////////////////////////////
    KeyTransition::KeyTransitionMap zeroTransiionProbabilities =
        KeyTransition("zero").getKeyTransitionMap();
    HiddenMarkovModel hmm2(
        keySequence,
        keyVector,
        initialProbabilities,
        zeroTransiionProbabilities,
        transitionProbabilities);
    Key::KeySequence mainKeySequence;
    hmm2.runViterbi();
    mainKeySequence = hmm2.getKeySequence();
}
