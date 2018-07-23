/*
MIT License

Copyright (c) 2018 Nestor Napoles

Adapted from Filip Jurcicek's implementation of the
viterbi algorithm:
http://bozskyfilip.blogspot.com/2009/01/viterbi-algorithm-in-c-and-using-stl.html

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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include<string>
#include<vector>
#include<map>
#include<iostream>

namespace hmm {

std::vector<std::string> states;
std::vector<std::string> observations;
std::map<std::string, double> start_probability;
std::map<std::string, std::map<std::string, double> > transition_probability;
std::map<std::string, std::map<std::string, double> > emission_probability;

class Tracking {
 public:
  double prob;
  std::vector<std::string> v_path;
  double v_prob;

  Tracking() {
    prob = 0.0;
    v_prob = 0.0;
  }

  Tracking(double p, const std::vector<std::string> & v_pth, double v_p) {
    prob = p;
    v_path = v_pth;
    v_prob = v_p;
  }
};

void init_variables(void) {
  states.push_back("Healthy");
  states.push_back("Fever");

  observations.push_back("normal");
  observations.push_back("cold");
  observations.push_back("dizzy");

  start_probability["Healthy"] = 0.6;
  start_probability["Fever"] = 0.4;

  transition_probability["Healthy"]["Healthy"] = 0.7;
  transition_probability["Healthy"]["Fever"] = 0.3;
  transition_probability["Fever"]["Healthy"] = 0.4;
  transition_probability["Fever"]["Fever"] = 0.6;

  emission_probability["Healthy"]["normal"] = 0.5;
  emission_probability["Healthy"]["cold"] = 0.4;
  emission_probability["Healthy"]["dizzy"] = 0.1;
  emission_probability["Fever"]["normal"] = 0.1;
  emission_probability["Fever"]["cold"] = 0.3;
  emission_probability["Fever"]["dizzy"] = 0.6;
}

void print_variables(void) {
  // print states
  std::cout << "States:" << std::endl;
  for (std::vector<std::string>::iterator i=states.begin();
    i != states.end(); i++) {
    std::cout << "S: " << (*i) << std::endl;
  }
  // print observations
  std::cout << "Observations:" << std::endl;
  for (std::vector<std::string>::iterator i=observations.begin();
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

void forward_viterbi(std::vector<std::string> obs,
    std::vector<std::string> states,
    std::map<std::string, double> start_p,
    std::map<std::string, std::map<std::string, double> > trans_p,
    std::map<std::string, std::map<std::string, double> > emit_p) {
  std::map<std::string, Tracking> T;

  for (std::vector<std::string>::iterator state=states.begin();
      state != states.end(); state++) {
    std::vector<std::string> v_pth;
    v_pth.push_back(*state);
    T[*state] = Tracking(start_p[*state], v_pth, start_p[*state]);
  }

  for (std::vector<std::string>::iterator output=obs.begin();
      output != obs.end(); output++) {
    std::map<std::string, Tracking> U;

    for (std::vector<std::string>::iterator next_state=states.begin();
        next_state != states.end(); next_state++) {
      Tracking next_tracker;

      for (std::vector<std::string>::iterator source_state=states.begin();
      source_state != states.end(); source_state++) {
        Tracking source_tracker = T[*source_state];

        double p = emit_p[*source_state][*output] *
            trans_p[*source_state][*next_state];
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

}  // namespace hmm

int main(int argc, char* argv[]) {
  std::cout << "Viterbi STL example" << std::endl;

  hmm::init_variables();
  hmm::print_variables();

  hmm::forward_viterbi(
    hmm::observations,
    hmm::states,
    hmm::start_probability,
    hmm::transition_probability,
    hmm::emission_probability);

  std::cout << "End" << std::endl;

  std::string end;
  std::cin >> end;

  return 0;
}
