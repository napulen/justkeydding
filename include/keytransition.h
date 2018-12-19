/*
MIT License

Copyright (c) 2018 Nestor Napoles

Key transition probability distributions for the hidden markov model

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

#ifndef INCLUDE_KEYTRANSITION_H_
#define INCLUDE_KEYTRANSITION_H_

#include<map>
#include<string>
#include<array>
#include<algorithm>

#include "./key.h"
#include "./pitchclass.h"

namespace justkeydding {

class KeyTransition {
 public:
    typedef std::array<double, Key::NUMBER_OF_KEYS> KeyTransitionArray;
    typedef std::map<Key, std::map<Key, double> > KeyTransitionMap;
    KeyTransition();
    explicit KeyTransition(std::string keyTransition);
    explicit KeyTransition(
         std::string keyTransition, KeyTransitionArray customKeyTransition);
    bool isValidKeyTransition(std::string keyTransition);
    std::string whichKeyTransition();
    KeyTransitionArray getKeyTransitionArray();
    KeyTransitionMap getKeyTransitionMap();

 private:
    std::string m_keyTransition;
    std::map<std::string, KeyTransitionArray> m_keyTransitions;
    void initKeyTransitionArrays();
};

}  // namespace justkeydding

#endif  // INCLUDE_KEYTRANSITION_H_
