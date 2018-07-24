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

using std::cout;
using std::endl;
using std::log10;

namespace justkeydding {

HiddenMarkovModel::HiddenMarkovModel(
  std::vector<PitchClass> observations,
  Key::KeyVector states,
  std::map<Key, double> initialProbabilities,
  std::map<Key, std::map<Key, double> > transitionProbabilities,
  std::map<Key, std::map<PitchClass, double> > emissionProbabilities) {
  // Observations
  for (std::vector<PitchClass>::const_iterator itObs = observations.begin();
      itObs != observations.end(); itObs++) {
    m_observations.push_back(itObs->getString());
  }
  // States
  for (std::vector<Key>::const_iterator itState = states.begin();
      itState != states.end(); itState++) {
    m_states.push_back(itState->getString());
  }
  // Initial probabilities
  for (std::map<Key, double>::const_iterator itInitial =
      initialProbabilities.begin(); itInitial != initialProbabilities.end();
      itInitial++) {
    std::string key = (itInitial->first).getString();
    m_initialProbabilities[key] =
        itInitial->second;
  }
  // Transition probabilities
  for (std::map<Key, std::map<Key,
      double> >::const_iterator itTransition = transitionProbabilities.begin();
      itTransition != transitionProbabilities.end(); itTransition++) {
    std::string fromKey = (itTransition->first).getString();
    std::map<Key, double> toKeyMap = itTransition->second;
    for (std::map<Key, double>::const_iterator itToKey = toKeyMap.begin();
        itToKey != toKeyMap.end(); itToKey++ ) {
      std::string toKey = (itToKey->first).getString();
      m_transitionProbabilities[fromKey][toKey] = itToKey->second;
    }
  }
  // Emission probabilities
  for (std::map<Key, std::map<PitchClass,
      double> >::const_iterator itEmission = emissionProbabilities.begin();
      itEmission != emissionProbabilities.end(); itEmission++) {
    std::string fromKey = (itEmission->first).getString();
    std::map<PitchClass, double> emitPcMap = itEmission->second;
    for (std::map<PitchClass, double>::const_iterator itEmitPc =
        emitPcMap.begin(); itEmitPc != emitPcMap.end(); itEmitPc++) {
      std::string emitPc = (itEmitPc->first).getString();
      m_emissionProbabilities[fromKey][emitPc] = itEmitPc->second;
    }
  }
}

HiddenMarkovModel::HiddenMarkovModel(
  std::vector<Key> observations,
  Key::KeyVector states,
  std::map<Key, double> initialProbabilities,
  std::map<Key, std::map<Key, double> > transitionProbabilities,
  std::map<Key, std::map<Key, double> > emissionProbabilities) {
  // Observations
  for (std::vector<Key>::const_iterator itObs = observations.begin();
      itObs != observations.end(); itObs++) {
    m_observations.push_back(itObs->getString());
  }
  // States
  for (std::vector<Key>::const_iterator itState = states.begin();
      itState != states.end(); itState++) {
    m_states.push_back(itState->getString());
  }
  // Initial probabilities
  for (std::map<Key, double>::const_iterator itInitial =
      initialProbabilities.begin(); itInitial != initialProbabilities.end();
      itInitial++) {
    std::string key = (itInitial->first).getString();
    m_initialProbabilities[key] =
        itInitial->second;
  }
  // Transition probabilities
  for (std::map<Key, std::map<Key,
      double> >::const_iterator itTransition = transitionProbabilities.begin();
      itTransition != transitionProbabilities.end(); itTransition++) {
    std::string fromKey = (itTransition->first).getString();
    std::map<Key, double> toKeyMap = itTransition->second;
    for (std::map<Key, double>::const_iterator itToKey = toKeyMap.begin();
        itToKey != toKeyMap.end(); itToKey++ ) {
      std::string toKey = (itToKey->first).getString();
      m_transitionProbabilities[fromKey][toKey] = itToKey->second;
    }
  }
  // Emission probabilities
  for (std::map<Key, std::map<Key,
      double> >::const_iterator itEmission = emissionProbabilities.begin();
      itEmission != emissionProbabilities.end(); itEmission++) {
    std::string fromKey = (itEmission->first).getString();
    std::map<Key, double> emitKeyMap = itEmission->second;
    for (std::map<Key, double>::const_iterator itEmitKey =
        emitKeyMap.begin(); itEmitKey != emitKeyMap.end(); itEmitKey++) {
      std::string emitKey = (itEmitKey->first).getString();
      m_emissionProbabilities[fromKey][emitKey] = itEmitKey->second;
    }
  }
}

void HiddenMarkovModel::printOutput() {
  // print states
  std::cout << "States:" << std::endl;
  for (std::vector<std::string>::const_iterator i = m_states.begin();
    i != m_states.end(); i++) {
    std::cout << "S: " << (*i) << std::endl;
  }

  // print observations
  std::cout << "Observations:" << std::endl;
  for (std::vector<std::string>::const_iterator i = m_observations.begin();
      i != m_observations.end(); i++) {
    std::cout << "O: " << (*i) << std::endl;
  }

  // print start probabilities
  std::cout << "Start probabilities:" << std::endl;
  for (std::map<std::string, double>::const_iterator i =
    m_initialProbabilities.begin(); i != m_initialProbabilities.end(); i++) {
    std::cout << "S: " << (*i).first << " P: " << (*i).second << std::endl;
  }

  // print transition_probability
  std::cout << "Transition probabilities:" << std::endl;
  for (std::map<std::string, std::map<std::string,
      double> >::const_iterator i = m_transitionProbabilities.begin();
      i != m_transitionProbabilities.end(); i++) {
    for (std::map<std::string, double>::const_iterator j =
        (*i).second.begin(); j != (*i).second.end(); j++) {
      std::cout << "FS: " << (*i).first << " TS: " <<
      (*j).first << " P: " << (*j).second << std::endl;
    }
  }

  // print emission probabilities
  std::cout << "Emission probabilities:" << std::endl;
  for (int i=0; i < m_states.size(); i++) {
    for (int j=0; j < m_observations.size(); j++) {
      std::cout
        << "FS: " << m_states[i] << " TO: " << m_observations[j] << " P: "
        << m_emissionProbabilities[m_states[i]][m_observations[j]]
        << std::endl;
    }
  }
}

HiddenMarkovModel::KeySequence HiddenMarkovModel::runViterbi() {
  KeySequence keySequence;
  std::map<std::string, Tracking> T;

  for (std::vector<std::string>::iterator state=m_states.begin();
      state != m_states.end(); state++) {
    std::vector<std::string> v_pth;
    v_pth.push_back(*state);
    T[*state] = Tracking(log10(m_initialProbabilities[*state]), v_pth,
        log10(m_initialProbabilities[*state]));
  }

  for (std::vector<std::string>::iterator output=m_observations.begin();
      output != m_observations.end(); output++) {
    std::map<std::string, Tracking> U;

    for (std::vector<std::string>::iterator next_state=m_states.begin();
        next_state != m_states.end(); next_state++) {
      Tracking next_tracker;

      for (std::vector<std::string>::iterator source_state=m_states.begin();
      source_state != m_states.end(); source_state++) {
        Tracking source_tracker = T[*source_state];

        double p = log10(m_emissionProbabilities[*source_state][*output]) +
            log10(m_transitionProbabilities[*source_state][*next_state]);
        source_tracker.prob += p;
        source_tracker.v_prob += p;

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

  for (std::vector<std::string>::iterator state = m_states.begin();
  state != m_states.end(); state++) {
    Tracking tracker = T[*state];
    final_tracker.prob += tracker.prob;
    if (tracker.v_prob > final_tracker.v_prob) {
      final_tracker.v_path = tracker.v_path;
      final_tracker.v_prob = tracker.v_prob;
    }
  }

  final_tracker.v_path.pop_back();

  for (std::vector<std::string>::const_iterator itNode =
    final_tracker.v_path.begin(); itNode != final_tracker.v_path.end();
    itNode++) {
    keySequence.push_back(Key(*itNode));
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

  return keySequence;
}

}  // namespace justkeydding
