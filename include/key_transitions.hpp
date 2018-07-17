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

#ifndef INCLUDE_KEY_TRANSITIONS_HPP_
#define INCLUDE_KEY_TRANSITIONS_HPP_

namespace hmm {

#define NUMBER_OF_KEYS 24

struct key_transition {
    double all_keys[NUMBER_OF_KEYS];
};

struct all_transitions {
    struct key_transition linear;
    struct key_transition exponential_2;
    struct key_transition exponential_10;
    struct key_transition zero;
    struct key_transition heatmap;
};

const struct all_key_transitions = {
    .linear = {
        .all_keys = {
            9/132, 4/132, 6/132, 6/132, 5/132, 8/132,
            1/132, 8/132, 5/132, 6/132, 6/132, 4/132,
            8/132, 2/132, 7/132, 3/132, 7/132, 7/132,
            3/132, 7/132, 2/132, 8/132, 5/132, 5/132,
        }
    },
    .exponential_2 = {
        .all_keys = {
            256/1245, 8/1245, 32/1245, 32/1245, 16/1245, 128/1245,
            1/1245, 128/1245, 16/1245, 32/1245, 32/1245, 8/1245,
            128/1245, 2/1245, 64/1245, 4/1245, 64/1245, 64/1245,
            4/1245, 64/1245, 2/1245, 128/1245, 16/1245, 16/1245,
        }
    },
    .exponential_10 = {
        .all_keys = {
            100000000/144442221, 1000/144442221, 100000/144442221,
            100000/144442221, 10000/144442221, 10000000/144442221,
            1/144442221, 10000000/144442221, 10000/144442221,
            100000/144442221, 100000/144442221, 1000/144442221,
            10000000/144442221, 10/144442221, 1000000/144442221,
            100/144442221, 1000000/144442221, 1000000/144442221,
            100/144442221, 1000000/144442221, 10/144442221,
            10000000/144442221, 10000/144442221, 10000/144442221
        }
    },
    .zero = {
        .all_keys = {
            1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        }
    },
    .heatmap = {
        .all_keys = {
            1, 6, 4, 4, 5, 2, 9, 2, 5, 4, 4, 6,
            2, 8, 3, 7, 3, 3, 7, 3, 8, 2, 5, 5
        }
    }
};

}  // namespace hmm

#endif  // INCLUDE_KEY_TRANSITIONS_HPP_
