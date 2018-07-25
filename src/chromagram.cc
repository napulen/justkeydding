/*
MIT License

Copyright (c) 2018 Nestor Napoles

Methods for obtaining chromagrams either from a CSV or Audio file

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

#include "./chromagram.h"

namespace justkeydding {

Chromagram::Chromagram(std::string fileName, enFileType fileType) {
    switch (fileType) {
        case FILETYPE_CSV:
            getChromagramFromCsv(fileName);
            break;
        case FILETYPE_AUDIO:
            getChromagramFromAudio(fileName);
            break;
    }
    discretizeChromagram();
}

PitchClass::PitchClassSequence Chromagram::getPitchClassSequence() {
    PitchClass::PitchClassSequence pitchClassSequence;
    for (ChromagramMap::const_iterator itChr = m_discreteChromagramMap.begin();
        itChr != m_discreteChromagramMap.end(); itChr++) {
        if (itChr->second == std::next(itChr)->second) {
            continue;
        }
        ChromagramVector chrVector = itChr->second;
        for (int pc = 0; pc < chrVector.size(); pc++) {
            int n = static_cast<int>(chrVector[pc]);
            do {
                pitchClassSequence.push_back(PitchClass(pc));
            } while (n--);
        }
    }
    return pitchClassSequence;
}

void Chromagram::printChromagram() {
    for (ChromagramMap::const_iterator itChr = m_discreteChromagramMap.begin();
         itChr != m_discreteChromagramMap.end(); itChr++) {
        double timestamp = itChr->first;
        ChromagramVector chrVector = itChr->second;
        std::cout << timestamp << ": ";
        for (ChromagramVector::const_iterator itChrVector = chrVector.begin();
            itChrVector != chrVector.end(); itChrVector++) {
            std::cout << (*itChrVector) << " ";
        }
        std::cout << std::endl;
    }
}

void Chromagram::getChromagramFromCsv(std::string csvFilename) {
    std::ifstream infile;
    infile.open(csvFilename);
    if (!infile.is_open()) {
        return;
    }
    std::string line;
    double timeStamp = 0.0;
    while (std::getline(infile, line)) {
        std::istringstream sstr(line);
        std::string token;
        int i = 0;
        while ( std::getline(sstr, token, ',') ) {
            if (i == 0) {
                timeStamp = std::stof(token);
            } else {
                // NNLS orders chromagrams from A-G#
                // we want to store them as C-B instead
                int pcIndex =
                    (PitchClass::PITCHCLASS_A_NATURAL + (i-1))
                    % PitchClass::NUMBER_OF_PITCHCLASSES;
                m_originalChromagramMap[timeStamp][pcIndex] =
                    std::stof(token);
            }
            i++;
        }
    }
    infile.close();
    return;
}

void Chromagram::getChromagramFromAudio(std::string fileName) {
    std::cout << "Can't, sorry." << std::endl;
    return;
}

void Chromagram::discretizeChromagram() {
    for (ChromagramMap::const_iterator itChr = m_originalChromagramMap.begin();
            itChr != m_originalChromagramMap.end(); itChr++) {
        ChromagramVector chrVector = itChr->second;
        for (int i = 0; i < chrVector.size(); i++) {
            double discreteMagnitude = std::floor(chrVector[i]);
            m_discreteChromagramMap[itChr->first][i] = discreteMagnitude;
        }
    }
    return;
}

}  // namespace justkeydding

