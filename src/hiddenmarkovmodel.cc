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

#include "./hiddenmarkovmodel.h"

namespace justkeydding {

HiddenMarkovModel::HiddenMarkovModel(
    std::vector<PitchClass> observations,
        Key::KeyVector keyVector states,
        std::map<Key, double> initialProbabilities,
        std::map<Key, std::map<Key, double> > transitionProbabilities,
        std::map<Key, std::map<PitchClass, double> > emissionProbabilities) :
        m_observations(observations),
        m_states(states),
        m_initialProbabilities(initialProbabilities),
        m_transitionProbabilities(transitionProbabilities),
        m_emissionProbabilities(emissionProbabilities) {}


void HiddenMarkovModel::printOutput() {
  // print states
  std::cout << "States:" << std::endl;
  for (std::vector<std::string>::iterator i=states.begin();
    i != states.end(); i++) {
    std::cout << "S: " << (*i) << std::endl;
  }
  // print observations
  std::cout << "Observations:" << std::endl;
  for (std::vector<int>::iterator i=observations.begin();
      i != observations.end(); i++) {
    std::cout << "O: " << (*i) << std::endl;
  }

  // print start probabilities
  std::cout << "Start probabilities:" << std::endl;
  for (std::map<std::string, double>::iterator i=start_probability.begin();
      i != start_probability.end(); i++) {
    std::cout << "S: " << (*i).first << " P: " << (*i).second << std::endl;
  }

  // print transition_probability
  std::cout << "Transition probabilities:" << std::endl;
  for (std::map<std::string, std::map<std::string, double> >::iterator i =
      transition_probability.begin(); i != transition_probability.end(); i++) {
    for (std::map<std::string, double>::iterator j = (*i).second.begin();
        j != (*i).second.end(); j++) {
      std::cout << "FS: " << (*i).first << " TS: " <<
      (*j).first << " P: " << (*j).second << std::endl;
    }
  }

  // print emission probabilities
  std::cout << "Emission probabilities:" << std::endl;
  for (int i=0; i < states.size(); i++) {
    for (int j=0; j < observations.size(); j++) {
      std::cout << "FS: " << states[i] << " TO: " << observations[j] <<
        " P: " << emission_probability[states[i]][observations[j]] << std::endl;
    }
  }
}

void HiddenMarkovModel::runViterbi() {
  std::map<std::string, Tracking> T;

  for (std::vector<std::string>::iterator state=m_states.begin();
      state != m_states.end(); state++) {
    std::vector<std::string> v_pth;
    v_pth.push_back(*state);
    T[*state] = Tracking(m_initialProbabilities[*state], v_pth,
        m_initialProbabilities[*state]);
  }

  for (std::vector<int>::iterator output=m_observations.begin();
      output != m_observations.end(); output++) {
    std::map<std::string, Tracking> U;

    for (std::vector<std::string>::iterator next_state=m_states.begin();
        next_state != m_states.end(); next_state++) {
      Tracking next_tracker;

      for (std::vector<std::string>::iterator source_state=m_states.begin();
      source_state != m_states.end(); source_state++) {
        Tracking source_tracker = T[*source_state];

        double p = m_emissionProbabilities[*source_state][*output] *
            m_transitionProbabilities[*source_state][*next_state];
        source_tracker.prob *= p;
        source_tracker.v_prob *= p;

        next_tracker.prob += source_tracker.prob;

        if (source_tracker.v_prob > next_tracker.v_prob) {
          next_tracker.v_path = source_tracker.v_path;
          next_tracker.v_path.push_back(*next_state);
          next_tracker.v_prob = source_tracker.v_prob;
        }
      }

      U[*next_state] = next_tracker;
    }
    T = U;
  }

  Tracking final_tracker;

  for (std::vector<std::string>::iterator state=states.begin();
  state != states.end(); state++) {
    Tracking tracker = T[*state];
    final_tracker.prob += tracker.prob;
    if (tracker.v_prob > final_tracker.v_prob) {
      final_tracker.v_path = tracker.v_path;
      final_tracker.v_prob = tracker.v_prob;
    }
  }

  std::cout << "Total probability of the observation sequence: "
       << final_tracker.prob << std::endl;
  std::cout << "Probability of the Viterbi path: "
       << final_tracker.v_prob << std::endl;
  std::cout << "The Viterbi path: " << std::endl;
  for (std::vector<std::string>::iterator state=final_tracker.v_path.begin();
  state != final_tracker.v_path.end(); state++) {
    std::cout << "VState: " << *state << std::endl;
  }
}


}  // namespace justkeydding
