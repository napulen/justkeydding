// ViterbiSTL.cpp : is an C++ and STL implementatiton of the Wikipedia example

// Wikipedia: http://en.wikipedia.org/wiki/Viterbi_algorithm#A_concrete_example

// It as accurate implementation as it was possible

#include "stdafx.h"

#include "string"
#include "vector"
#include "map"
#include "iostream"

using namespace std;

//states = ('Rainy', 'Sunny')
// 
//observations = ('walk', 'shop', 'clean')
// 
//start_probability = {'Rainy': 0.6, 'Sunny': 0.4}
// 
//transition_probability = {
//   'Rainy' : {'Rainy': 0.7, 'Sunny': 0.3},
//   'Sunny' : {'Rainy': 0.4, 'Sunny': 0.6},
//   }
// 
//emission_probability = {
//   'Rainy' : {'walk': 0.1, 'shop': 0.4, 'clean': 0.5},
//   'Sunny' : {'walk': 0.6, 'shop': 0.3, 'clean': 0.1},
//   }

vector<string> states;
vector<string> observations;
map<string,double> start_probability;
map<string,map<string, double>> transition_probability;
map<string,map<string, double>> emission_probability;

class Tracking {
public:
  double prob;
  vector<string> v_path;
  double v_prob;

  Tracking() {
    prob = 0.0;
    v_prob = 0.0;
  }

  Tracking(double p, vector<string> & v_pth, double v_p) {
    prob = p;
    v_path = v_pth;
    v_prob = v_p;
  }
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void init_variables(void) {
  states.push_back("Rainy");
	states.push_back("Sunny");

  observations.push_back("walk");
  observations.push_back("shop");
  observations.push_back("clean");

  start_probability["Rainy"] = 0.6;
  start_probability["Sunny"] = 0.4;

  transition_probability["Rainy"]["Rainy"] = 0.7;
  transition_probability["Rainy"]["Sunny"] = 0.3;
  transition_probability["Sunny"]["Rainy"] = 0.4;
  transition_probability["Sunny"]["Sunny"] = 0.6;

  emission_probability["Rainy"]["walk"] = 0.1;
  emission_probability["Rainy"]["shop"] = 0.4;
  emission_probability["Rainy"]["clean"] = 0.5;
  emission_probability["Sunny"]["walk"] = 0.6;
  emission_probability["Sunny"]["shop"] = 0.3;
  emission_probability["Sunny"]["clean"] = 0.1;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void print_variables(void) {
  // print states
  cout << "States:" << endl;
  for(vector<string>::iterator i=states.begin();i!=states.end();i++) {
    cout << "S: " << (*i) << endl;
  }
  // print observations
  cout << "Observations:" << endl;
  for(vector<string>::iterator i=observations.begin();i!=observations.end();i++) {
    cout << "O: " << (*i) << endl;
  }

  // print start probabilities
  cout << "Start probabilities:" << endl;
  for(map<string, double>::iterator i=start_probability.begin();i!=start_probability.end();i++) {
    cout << "S: " << (*i).first << " P: " << (*i).second << endl;
  }

  // print transition_probability
  cout << "Transition probabilities:" << endl;
  for(map<string,map<string, double>>::iterator i=transition_probability.begin();i!=transition_probability.end();i++) {
    for(map<string, double>::iterator j=(*i).second.begin();j!=(*i).second.end();j++) {
      cout << "FS: " << (*i).first << " TS: " << (*j).first << " P: " << (*j).second << endl;
    }
  }

  // print emission probabilities
  cout << "Emission probabilities:" << endl;
  for(int i=0; i<states.size(); i++) {
    for(int j=0; j<observations.size(); j++) {
      cout << "FS: " << states[i] << " TO: " << observations[j] <<
        " P: " << emission_probability[states[i]][observations[j]] << endl;
    }
  }
}

//this method compute total probability for observation, most likely viterbi path 
//and probability of such path
void forward_viterbi(vector<string> obs, vector<string> states, map<string, double> start_p, 
                     map<string, map<string, double>> trans_p, 
                     map<string, map<string, double>> emit_p) {
  map<string, Tracking> T;

  for(vector<string>::iterator state=states.begin(); state!=states.end();state++) {
    vector<string> v_pth;
    v_pth.push_back(*state);

    T[*state] = Tracking(start_p[*state], v_pth, start_p[*state]);
  }

  for(vector<string>::iterator output=obs.begin(); output!=obs.end();output++) {
    map<string, Tracking> U;

    for(vector<string>::iterator next_state=states.begin(); next_state!=states.end(); next_state++) {
      Tracking next_tracker;

      for(vector<string>::iterator source_state=states.begin(); source_state!=states.end(); source_state++) {
        Tracking source_tracker = T[*source_state];

        double p = emit_p[*source_state][*output]*trans_p[*source_state][*next_state];
        source_tracker.prob *= p;
        source_tracker.v_prob *= p;

        next_tracker.prob += source_tracker.prob;

        if(source_tracker.v_prob > next_tracker.v_prob) {
          next_tracker.v_path = source_tracker.v_path;
          next_tracker.v_path.push_back(*next_state);
          next_tracker.v_prob = source_tracker.v_prob;
        }
      }

      U[*next_state] = next_tracker;
    }

    T = U;
  }

  // apply sum/max to the final states
  Tracking final_tracker;

  for(vector<string>::iterator state=states.begin(); state!=states.end(); state++) {
    Tracking tracker = T[*state];

    final_tracker.prob += tracker.prob;

    if(tracker.v_prob > final_tracker.v_prob) {
      final_tracker.v_path = tracker.v_path;
      final_tracker.v_prob = tracker.v_prob;
    }
  }

  cout << "Total probability of the observation sequence: " << final_tracker.prob << endl;
  cout << "Probability of the Viterbi path: " << final_tracker.v_prob << endl;
  cout << "The Viterbi path: " << endl;
  for(vector<string>::iterator state=final_tracker.v_path.begin(); state!=final_tracker.v_path.end(); state++) {
    cout << "VState: " << *state << endl;
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  cout << "Viterbi STL example" << endl;
  
  init_variables();
  print_variables();

  forward_viterbi(observations, 
                  states, 
                  start_probability, 
                  transition_probability, 
                  emission_probability);

	cout << "End" << endl;

  string end;
  cin >> end;

  return 0;
}

