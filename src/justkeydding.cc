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

enum status {
    STATUS_OK,
    STATUS_MISSING_ARG,
    STATUS_FILE_ERROR
};

#define PITCH_CLASSES 12

typedef std::array<float, PITCH_CLASSES> pitchClassArray;

struct chromagram {
    float time;
    pitchClassArray magnitudes;
};

typedef std::vector<chromagram> chromagramVector;
typedef std::vector<int> pitchClassSequence;

char *programName;

int getChromagrams(char *audioFile, chromagramVector *chromagrams) {
    std::ifstream infile;
    infile.open(audioFile);
    if (!infile.is_open()) {
        return STATUS_FILE_ERROR;
    }
    std::string line;
    while ( std::getline(infile, line) ) {
        std::istringstream sstr(line);
        std::string token;
        int i = 0;
        struct chromagram chr;
        while ( std::getline(sstr, token, ',') ) {
            if (!i) {
                chr.time = std::stof(token);
            } else {
                // NNLS orders chromagrams from A-G#
                // we want to store them as C-B instead
                int pcIndex = (9 + (i-1)) % PITCH_CLASSES;
                chr.magnitudes[pcIndex] = std::stof(token);
            }
            i++;
        }
        chromagrams->push_back(chr);
    }
    infile.close();
    return STATUS_OK;
}

int normalizeChromagrams(chromagramVector *chromagrams) {
    // For now, as a normalization, we just get the integer
    // value of the magnitude and discard the rest
    for (chromagramVector::iterator it = chromagrams->begin();
         it != chromagrams->end();
         it++) {
        for (int pc=0; pc < 12; pc++) {
            int pcMagnitude = static_cast<int>(it->magnitudes[pc]);
            it->magnitudes[pc] = static_cast<float>(pcMagnitude);
        }
    }
    return STATUS_OK;
}

int getPitchClassSequence(const chromagramVector &chromagrams,
    pitchClassSequence *pcSeq) {
    for (chromagramVector::const_iterator it = chromagrams.begin();
         it != chromagrams.end();
         it++) {
        if (it->magnitudes == (it+1)->magnitudes) {
            // Remove duplicates
            continue;
        }
        for (int pc=0; pc < 12; pc++) {
            int pcMagnitude = static_cast<int>(it->magnitudes[pc]);
            if (pcMagnitude) {
                for (int n=0; n < pcMagnitude; n++) {
                    pcSeq->push_back(pc);
                }
            }
        }
    }
    return STATUS_OK;
}

int printChromagram(const chromagramVector &chromagrams) {
    for (chromagramVector::const_iterator it = chromagrams.begin();
         it != chromagrams.end();
         it++) {
        int pc;
        std::cout << it->time << ", ";
        for (pc=0; pc < 11; pc++) {
            std::cout << it->magnitudes[pc] << ", ";
        }
        std::cout << it->magnitudes[pc] << std::endl;
    }
}

int printPitchClassSequence(const pitchClassSequence &pcSeq) {
    for (pitchClassSequence::const_iterator it = pcSeq.begin();
         it != pcSeq.end();
         it++) {
        std::cout << *it << ",";
    }
    std::cout << std::endl;
}

void logError(int status) {
    std::string msg;
    switch (status) {
        case STATUS_MISSING_ARG:
            msg = "Missing an argument.";
            break;
        case STATUS_FILE_ERROR:
            msg = "Couldn't open your file.";
            break;
        default:
        msg = "There was an error running the program.";
    }
    msg += "\n\nUsage:\n" + std::string(programName) + " <input_file>";
    std::cerr << "Error " << status << ": " << msg << std::endl;
}

int main(int argc, char *argv[]) {
    int status;
    // This should always exist, right?
    programName = argv[0];
    if (argc != 2) {
        // TODO(napulen): Add a proper argparse and help/usage
        status = STATUS_MISSING_ARG;
        logError(status);
        return status;
    }
    chromagramVector chromagrams;
    // Get the chromagrams
    if ((status = getChromagrams(argv[1], &chromagrams)) != STATUS_OK) {
        logError(status);
        return status;
    }
    // "Normalizing" the chromagrams
    if ((status = normalizeChromagrams(&chromagrams)) != STATUS_OK) {
        logError(status);
        return status;
    }
    pitchClassSequence pcSequence;
    // Turning the chromagrams into a sequence of pitch-classes
    if ((status = getPitchClassSequence(chromagrams,
        &pcSequence)) != STATUS_OK) {
        logError(status);
        return status;
    }
    printPitchClassSequence(pcSequence);
    return STATUS_OK;
}
