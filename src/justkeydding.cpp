/* Copyright 2018 Néstor Nápoles 
For now, the code is only reading a csv with the features already
extracted (e.g., using nnls-chroma or essentia). This should be
substituted in the future for the chromagram extraction done here.

Some things to do:
  * Embed chromagram computation from nnls
  * Store the output in some data structure
  * Implement a viterbi algorithm to find the best path
  * Import the weights I computed in the symbolic version
  * Generate a cool dataset to test this before submission */

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
                chr.magnitudes[i-1] = std::stof(token);
            }
            i++;
        }
        chromagrams->push_back(chr);
    }
    infile.close();
    return STATUS_OK;
}

int preprocessChromagrams(chromagramVector *chromagrams) {
    for (chromagramVector::const_iterator it = chromagrams->begin();
         it != chromagrams->end();
         it++) {
        if (it->magnitudes == (it+1)->magnitudes) {
            // Remove duplicates
            continue;
        }
        for (int pc=0; pc < 12; pc++) {
            // The original order of the chromagram
            // pitch-classes is A-G#, we want it to be C-B
            int pcIndex = (3+pc) % PITCH_CLASSES;
            float pcMagnitude = it->magnitudes[pcIndex];
            std::cout << pcMagnitude << " ";
        }
        std::cout << std::endl;
    }
    return STATUS_OK;
}

int getMagnitude(float magnitude) {
    // This function determines how to "discretize"
    // the magnitude of a pitch-class in the chromagram
    return static_cast<int>(magnitude);
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
    // Any pre-processing before "discretizing"
    if ((status = preprocessChromagrams(&chromagrams)) != STATUS_OK) {
        logError(status);
        return status;
    }
    return STATUS_OK;
}
