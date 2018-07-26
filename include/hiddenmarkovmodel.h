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

#ifndef INCLUDE_HIDDENMARKOVMODEL_H_
#define INCLUDE_HIDDENMARKOVMODEL_H_

#include<string>
#include<vector>
#include<map>
#include<unordered_map>
#include<cmath>
#include<iostream>
#include<limits>

#include "./pitchclass.h"
#include "./key.h"
#include "./keyprofile.h"
#include "./keytransition.h"


namespace justkeydding {

class Tracking {
 public:
  std::vector<int> v_path;
  double v_prob;

  Tracking() {
    v_prob = -std::numeric_limits<double>::infinity();
  }

  Tracking(const std::vector<int> & v_pth, double v_p) {
    v_path = v_pth;
    v_prob = v_p;
  }
};


class HiddenMarkovModel {
    std::vector<int> m_observations;
    std::vector<int> m_states;
    std::unordered_map<int, double> m_initialProbabilities;
    std::unordered_map<int,
        std::unordered_map<int, double> > m_transitionProbabilities;
    std::unordered_map<int,
        std::unordered_map<int, double> > m_emissionProbabilities;

    struct ViterbiNode {
      double probability;
      int previousState;
      ViterbiNode(
        double p = -std::numeric_limits<double>::infinity(),
        int s = -1) : probability(p), previousState(s) { }
    };

 public:
    typedef std::unordered_map<int, ViterbiNode> ViterbiLayer;
    std::vector<ViterbiLayer> m_viterbi;
    HiddenMarkovModel(
      std::vector<PitchClass> observations,
      Key::KeyVector states,
      std::map<Key, double> initialProbabilities,
      std::map<Key, std::map<Key, double> > transitionProbabilities,
      std::map<Key, std::map<PitchClass, double> > emissionProbabilities);
    HiddenMarkovModel(
      std::vector<Key> observations,
      Key::KeyVector states,
      std::map<Key, double> initialProbabilities,
      std::map<Key, std::map<Key, double> > transitionProbabilities,
      std::map<Key, std::map<Key, double> > emissionProbabilities);
    void printOutput();
    Key::KeySequence runViterbi();
};


}  // namespace justkeydding

#endif  // INCLUDE_HIDDENMARKOVMODEL_H_
