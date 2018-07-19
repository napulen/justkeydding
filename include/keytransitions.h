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

#ifndef INCLUDE_KEYTRANSITIONS_H_
#define INCLUDE_KEYTRANSITIONS_H_

namespace hmm {

#define NUMBER_OF_KEYS 24

struct key_transition {
    double array[NUMBER_OF_KEYS];
};

struct key_transition_collection {
    struct key_transition linear;
    struct key_transition exponential_2;
    struct key_transition exponential_10;
    struct key_transition zero;
    struct key_transition heatmap;
};

const struct key_transition_collection key_transitions = {
    .linear = {
        .array = {
            9.0/132, 4.0/132, 6.0/132, 6.0/132, 5.0/132, 8.0/132,
            1.0/132, 8.0/132, 5.0/132, 6.0/132, 6.0/132, 4.0/132,
            8.0/132, 2.0/132, 7.0/132, 3.0/132, 7.0/132, 7.0/132,
            3.0/132, 7.0/132, 2.0/132, 8.0/132, 5.0/132, 5.0/132,
        }
    },
    .exponential_2 = {
        .array = {
            256.0/1245, 8.0/1245, 32.0/1245, 32.0/1245, 16.0/1245, 128.0/1245,
            1.0/1245, 128.0/1245, 16.0/1245, 32.0/1245, 32.0/1245, 8.0/1245,
            128.0/1245, 2.0/1245, 64.0/1245, 4.0/1245, 64.0/1245, 64.0/1245,
            4.0/1245, 64.0/1245, 2.0/1245, 128.0/1245, 16.0/1245, 16.0/1245,
        }
    },
    .exponential_10 = {
        .array = {
            100000000.0/144442221, 1000.0/144442221, 100000.0/144442221,
            100000.0/144442221, 10000.0/144442221, 10000000.0/144442221,
            1.0/144442221, 10000000.0/144442221, 10000.0/144442221,
            100000.0/144442221, 100000.0/144442221, 1000.0/144442221,
            10000000.0/144442221, 10.0/144442221, 1000000.0/144442221,
            100.0/144442221, 1000000.0/144442221, 1000000.0/144442221,
            100.0/144442221, 1000000.0/144442221, 10.0/144442221,
            10000000.0/144442221, 10000.0/144442221, 10000.0/144442221
        }
    },
    .zero = {
        .array = {
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        }
    },
    .heatmap = {
        .array = {
            1.0, 6.0, 4.0, 4.0, 5.0, 2.0, 9.0, 2.0, 5.0, 4.0, 4.0, 6.0,
            2.0, 8.0, 3.0, 7.0, 3.0, 3.0, 7.0, 3.0, 8.0, 2.0, 5.0, 5.0
        }
    }
};

}  // namespace hmm

#endif  // INCLUDE_KEYTRANSITIONS_H_
