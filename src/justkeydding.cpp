/* June 2018, Nestor Napoles 
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

#define RTRN_MISSING_ARG 1
#define RTRN_FILE_ERROR 2

#define PITCH_CLASSES 12

struct entry{
    float time;
    std::array<int, 12> magnitudes;
};

int getMagnitude(float magnitude){
    return (int)magnitude;
}

/* For now, this function reads a csv with computed chromagram
   features and starts parsing that into a sequence of observa-
   tion symbols for the HMM */
int main(int argc, char *argv[]) {
    // Parsing args
    if(argc != 2) {
        std::cout << "Missing an argument" 
                  << std::endl << argv[0] 
                  << "<input_file>" << std::endl;
        // TODO: Add a proper argparse and help
        return RTRN_MISSING_ARG;
    }
    // Reading the csv
    std::ifstream infile;
    infile.open(argv[1]);    
    if(!infile.is_open()) {
        std::cout << "Couldn't open your file." << std::endl;
        return RTRN_FILE_ERROR;
    }
    std::string line;
    std::vector<entry> rows;
    while(std::getline(infile, line)) {
        std::istringstream sstr(line);
        std::string token;
        int i = 0;
        struct entry x;
        while(std::getline(sstr, token, ',')) {            
            if(!i){
                x.time = std::stof(token);          
            } else{
                x.magnitudes[i-1] = getMagnitude(std::stof(token));
            }
            i++;
        }
        if(rows.empty()){
            rows.push_back(x);
        } else if(x.magnitudes != rows.back().magnitudes){
            rows.push_back(x);
        }
    }
    for(std::vector<entry>::const_iterator it = rows.begin(); 
        it != rows.end(); it++) {
        std::cout << it->time << ":" 
                  << std::endl << "\t";     
        for(int pc=0; pc < 12; pc++){
            int pcIndex = (3+pc) % PITCH_CLASSES;
            int pcMagnitude = it->magnitudes[pcIndex];
            std::cout << (int)pcMagnitude << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}