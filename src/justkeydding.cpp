/* Copyright 2018 Nestor Napoles 
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

#define RTRN_OK 0
#define RTRN_MISSING_ARG 1
#define RTRN_FILE_ERROR 2

#define PITCH_CLASSES 12

struct chromagram {
    float time;
    std::array<int, 12> magnitudes;
};

char *programName;

int getChromagram(char *audioFile, std::vector<chromagram> *chromagrams) {
    std::ifstream infile;
    infile.open(audioFile);
    if (!infile.is_open()) {
        return RTRN_FILE_ERROR;
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
    return RTRN_OK;
}

int getMagnitude(float magnitude) {
    // This function determines how to "discretize"
    // the magnitude of a pitch-class in the chromagram
    return static_cast<int>(magnitude);
}

void logError(int status) {
    std::string msg;
    switch (status) {
        case RTRN_MISSING_ARG:
            msg = "Missing an argument.";
            break;
        case RTRN_FILE_ERROR:
            msg = "Couldn't open your file.";
            break;
        default:
        msg = "There was an error running the program.";
    }
    msg += "\n\nUsage:\n" + std::string(programName) + " <input_file>\n";
    std::cerr << "Error " << status << ": " << msg << std::endl;
}

int main(int argc, char *argv[]) {
    // Parsing args
    int status;
    // This should always exist, right?
    programName = argv[0];
    if (argc != 2) {
        // TODO(napulen): Add a proper argparse and help
        status = RTRN_MISSING_ARG;
        logError(status);
        return status;
    }
    std::vector<chromagram> chromagrams;
    if ((status = getChromagram(argv[1], &chromagrams)) != RTRN_OK) {
        logError(status);
        return status;
    }
    for (std::vector<chromagram>::const_iterator it = chromagrams.begin();
         it != chromagrams.end();
         it++) {
        std::cout << it->time << ":"
                  << std::endl << "\t";
        for (int pc=0; pc < 12; pc++) {
            // The original order of the chromagram
            // pitch-classes is A-G#, we want it to be C-B
            int pcIndex = (3+pc) % PITCH_CLASSES;
            int pcMagnitude = it->magnitudes[pcIndex];
            std::cout << static_cast<int>(pcMagnitude) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
