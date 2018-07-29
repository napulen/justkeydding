/*
MIT License

Copyright (c) 2018 Nestor Napoles

Status object to be used by all the modules of the key detector.
For now, it is nothing else but an enum.

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

#ifndef INCLUDE_STATUS_H_
#define INCLUDE_STATUS_H_

namespace justkeydding {

class Status {
 public:
  enum enStatus {
    CHROMAGRAM_UNINITIALIZED,
    CHROMAGRAM_INPUTFILE_ERROR,
    CHROMAGRAM_NNLS_ERROR,
    CHROMAGRAM_ORIGINAL_READY,
    CHROMAGRAM_DISCRETE_READY,
  };
};

}  // namespace justkeydding

#endif  // INCLUDE_STATUS_H_
