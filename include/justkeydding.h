/*
MIT License

Copyright (c) 2018 Nestor Napoles

Audio key detection program based on chromagrams and a hidden markov model (HMM)

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

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<array>

#include "./pitchclass.h"
#include "./key.h"
#include "./keyprofile.h"
#include "./keytransition.h"
#include "./chromagram.h"
#include "./hiddenmarkovmodel.h"

namespace justkeydding {

enum status {
    STATUS_OK,
    STATUS_MISSING_ARG,
    STATUS_FILE_ERROR
};

// TODO(napulen): Make an actual logger as a separate header
//               and source that everyone should include
// void logError(int status) {
//     std::string msg;
//     switch (status) {
//         case STATUS_MISSING_ARG:
//             msg = "Missing an argument.";
//             break;
//         case STATUS_FILE_ERROR:
//             msg = "Couldn't open your file.";
//             break;
//         default:
//         msg = "There was an error running the program.";
//     }
//     msg += "\n\nUsage:\n" + std::string(programName) + " <input_file>";
//     std::cerr << "Error " << status << ": " << msg << std::endl;
// }

}  // namespace justkeydding
