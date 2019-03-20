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
    m_observations.push_back(itObs->getInt());
  }
  // States
  for (std::vector<Key>::const_iterator itState = states.begin();
      itState != states.end(); itState++) {
    m_states.push_back(itState->getInt());
  }
  // Initial probabilities
  for (std::map<Key, double>::const_iterator itInitial =
      initialProbabilities.begin(); itInitial != initialProbabilities.end();
      itInitial++) {
    int key = (itInitial->first).getInt();
    m_initialProbabilities[key] =
        itInitial->second;
  }
  // Transition probabilities
  for (std::map<Key, std::map<Key,
      double> >::const_iterator itTransition = transitionProbabilities.begin();
      itTransition != transitionProbabilities.end(); itTransition++) {
    int fromKey = (itTransition->first).getInt();
    std::map<Key, double> toKeyMap = itTransition->second;
    for (std::map<Key, double>::const_iterator itToKey = toKeyMap.begin();
        itToKey != toKeyMap.end(); itToKey++ ) {
      int toKey = (itToKey->first).getInt();
      m_transitionProbabilities[fromKey][toKey] = itToKey->second;
    }
  }
  // Emission probabilities
  for (std::map<Key, std::map<PitchClass,
      double> >::const_iterator itEmission = emissionProbabilities.begin();
      itEmission != emissionProbabilities.end(); itEmission++) {
    int fromKey = (itEmission->first).getInt();
    std::map<PitchClass, double> emitPcMap = itEmission->second;
    for (std::map<PitchClass, double>::const_iterator itEmitPc =
        emitPcMap.begin(); itEmitPc != emitPcMap.end(); itEmitPc++) {
      int emitPc = (itEmitPc->first).getInt();
      m_emissionProbabilities[fromKey][emitPc] = itEmitPc->second;
    }
  }
  m_status = Status::HIDDENMARKOVMODEL_UNINITIALIZED;
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
    m_observations.push_back(itObs->getInt());
  }
  // States
  for (std::vector<Key>::const_iterator itState = states.begin();
      itState != states.end(); itState++) {
    m_states.push_back(itState->getInt());
  }
  // Initial probabilities
  for (std::map<Key, double>::const_iterator itInitial =
      initialProbabilities.begin(); itInitial != initialProbabilities.end();
      itInitial++) {
    int key = (itInitial->first).getInt();
    m_initialProbabilities[key] =
        itInitial->second;
  }
  // Transition probabilities
  for (std::map<Key, std::map<Key,
      double> >::const_iterator itTransition = transitionProbabilities.begin();
      itTransition != transitionProbabilities.end(); itTransition++) {
    int fromKey = (itTransition->first).getInt();
    std::map<Key, double> toKeyMap = itTransition->second;
    for (std::map<Key, double>::const_iterator itToKey = toKeyMap.begin();
        itToKey != toKeyMap.end(); itToKey++ ) {
      int toKey = (itToKey->first).getInt();
      m_transitionProbabilities[fromKey][toKey] = itToKey->second;
    }
  }
  // Emission probabilities
  for (std::map<Key, std::map<Key,
      double> >::const_iterator itEmission = emissionProbabilities.begin();
      itEmission != emissionProbabilities.end(); itEmission++) {
    int fromKey = (itEmission->first).getInt();
    std::map<Key, double> emitKeyMap = itEmission->second;
    for (std::map<Key, double>::const_iterator itEmitKey =
        emitKeyMap.begin(); itEmitKey != emitKeyMap.end(); itEmitKey++) {
      int emitKey = (itEmitKey->first).getInt();
      m_emissionProbabilities[fromKey][emitKey] = itEmitKey->second;
    }
  }
  m_status = Status::HIDDENMARKOVMODEL_UNINITIALIZED;
}

void HiddenMarkovModel::printOutput() {
  // print states
  std::cout << "States:" << std::endl;
  for (States::const_iterator i = m_states.begin();
    i != m_states.end(); i++) {
    std::cout << "S: " << (*i) << std::endl;
  }

  // print observations
  std::cout << "Observations:" << std::endl;
  for (Observations::const_iterator i = m_observations.begin();
      i != m_observations.end(); i++) {
    std::cout << "O: " << (*i) << std::endl;
  }

  // print start probabilities
  std::cout << "Start probabilities:" << std::endl;
  for (InitialProbabilities::const_iterator i =
    m_initialProbabilities.begin(); i != m_initialProbabilities.end(); i++) {
    std::cout << "S: " << (*i).first << " P: " << (*i).second << std::endl;
  }

  // print transition_probability
  std::cout << "Transition probabilities:" << std::endl;
  for (TransitionProbabilities::const_iterator i =
    m_transitionProbabilities.begin();
    i != m_transitionProbabilities.end(); i++) {
    for (TransitionProbability::const_iterator j =
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

void HiddenMarkovModel::runViterbi() {
  std::vector<ViterbiLayer> m_viterbi;
  ViterbiLayer currentLayer;
  ViterbiNode currentNode;
  double transition, emission, trialProbability;
  double maximumProbability = -std::numeric_limits<double>::infinity();
  for (std::vector<int>::const_iterator itState=m_states.begin();
      itState != m_states.end(); itState++) {
    int firstObservation = m_observations[0];
    transition = m_initialProbabilities[*itState];
    emission = m_emissionProbabilities[*itState][firstObservation];
    currentNode.probability = log10(transition * emission);
    currentLayer[*itState] = currentNode;
  }
  m_viterbi.push_back(currentLayer);
  for (std::vector<int>::const_iterator itObservation =
      std::next(m_observations.begin());
      itObservation != m_observations.end(); itObservation++) {
    ViterbiLayer nextLayer;
    for (std::vector<int>::const_iterator itNextState=m_states.begin();
        itNextState != m_states.end(); itNextState++) {
      ViterbiNode nextNode;
      for (std::vector<int>::const_iterator itCurrentState=m_states.begin();
      itCurrentState != m_states.end(); itCurrentState++) {
        currentNode = currentLayer[*itCurrentState];
        transition = m_transitionProbabilities[*itCurrentState][*itNextState];
        emission = m_emissionProbabilities[*itNextState][*itObservation];
        trialProbability =
          currentNode.probability + log10(transition * emission);
        if (trialProbability > nextNode.probability) {
          nextNode.probability = trialProbability;
          nextNode.previousState = *itCurrentState;
        }
      }
      nextLayer[*itNextState] = nextNode;
    }
    currentLayer = nextLayer;
    m_viterbi.push_back(currentLayer);
  }
  ViterbiLayer lastLayer;
  int lastState = -1;
  ProbabilityVector probabilities = ProbabilityVector(24, 0);
  while (!m_viterbi.empty()) {
    lastLayer = m_viterbi.back();
    if (lastState == -1) {
      for (ViterbiLayer::const_iterator itLastState = lastLayer.begin();
          itLastState != lastLayer.end(); itLastState++) {
        ViterbiNode lastNode = itLastState->second;
        probabilities[itLastState->first] = lastNode.probability;
        if (lastNode.probability > maximumProbability) {
          maximumProbability = lastNode.probability;
          lastState = itLastState->first;
        }
      }
    }
    m_keySequence.push_back(Key(lastState));
    currentNode = lastLayer.at(lastState);
    lastState = currentNode.previousState;
    m_viterbi.pop_back();
  }
  std::reverse(m_keySequence.begin(), m_keySequence.end());
  m_maximumProbability = maximumProbability;
  m_probabilityVector = probabilities;
  m_status = Status::HIDDENMARKOVMODEL_VITERBI_READY;
  return;
}

Key::KeySequence HiddenMarkovModel::getKeySequence() {
  return m_keySequence;
}

HiddenMarkovModel::ProbabilityVector HiddenMarkovModel::getProbabilityVector() const {
  return m_probabilityVector;
}

double HiddenMarkovModel::getMaximumProbability() const {
  return m_maximumProbability;
}

int HiddenMarkovModel::getStatus() const {
  return m_status;
}

}  // namespace justkeydding
