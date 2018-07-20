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

#include<map>
#include<string>
#include<array>

#include "./keytransition.h"

namespace justkeydding {

KeyTransition::KeyTransition() :
    KeyTransition("exponential10") {}

KeyTransition::KeyTransition(std::string keyTransition) {
    initKeyTransitionArrays();
    if (isValidKeyTransition(keyTransition)) {
        m_keyTransition = keyTransition;
    }
}

bool KeyTransition::isValidKeyTransition(std::string keyTransition) {
    return m_keyTransitions.find(keyTransition) != m_keyTransitions.end();
}

std::string KeyTransition::whichKeyTransition() {
    return m_keyTransition;
}

KeyTransition::KeyTransitionArray KeyTransition::getKeyTransition() {
    return m_keyTransitions[m_keyTransition];
}

void KeyTransition::initKeyTransitionArrays() {
    m_keyTransitions["linear"] = {
        9.0/132, 4.0/132, 6.0/132, 6.0/132, 5.0/132, 8.0/132,
        1.0/132, 8.0/132, 5.0/132, 6.0/132, 6.0/132, 4.0/132,
        8.0/132, 2.0/132, 7.0/132, 3.0/132, 7.0/132, 7.0/132,
        3.0/132, 7.0/132, 2.0/132, 8.0/132, 5.0/132, 5.0/132,
    };
    m_keyTransitions["exponential2"] = {
        256.0/1245, 8.0/1245, 32.0/1245, 32.0/1245, 16.0/1245, 128.0/1245,
        1.0/1245, 128.0/1245, 16.0/1245, 32.0/1245, 32.0/1245, 8.0/1245,
        128.0/1245, 2.0/1245, 64.0/1245, 4.0/1245, 64.0/1245, 64.0/1245,
        4.0/1245, 64.0/1245, 2.0/1245, 128.0/1245, 16.0/1245, 16.0/1245,
    };
    m_keyTransitions["exponential10"] = {
        100000000.0/144442221, 1000.0/144442221, 100000.0/144442221,
        100000.0/144442221, 10000.0/144442221, 10000000.0/144442221,
        1.0/144442221, 10000000.0/144442221, 10000.0/144442221,
        100000.0/144442221, 100000.0/144442221, 1000.0/144442221,
        10000000.0/144442221, 10.0/144442221, 1000000.0/144442221,
        100.0/144442221, 1000000.0/144442221, 1000000.0/144442221,
        100.0/144442221, 1000000.0/144442221, 10.0/144442221,
        10000000.0/144442221, 10000.0/144442221, 10000.0/144442221,
    };
    m_keyTransitions["zero"] = {
        1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    };
    m_keyTransitions["symmetrical"] = {
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
        1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0, 1.0/24.0,
    };
    m_keyTransitions["heatmap"] = {
        1.0, 6.0, 4.0, 4.0, 5.0, 2.0, 9.0, 2.0, 5.0, 4.0, 4.0, 6.0,
        2.0, 8.0, 3.0, 7.0, 3.0, 3.0, 7.0, 3.0, 8.0, 2.0, 5.0, 5.0,
    };
}

}  // namespace justkeydding
