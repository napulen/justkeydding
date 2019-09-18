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
    m_status = Status::CHROMAGRAM_UNINITIALIZED;
    switch (fileType) {
        case FILETYPE_CSV:
            getChromagramFromCsv(fileName);
            break;
        case FILETYPE_AUDIO:
            getChromagramFromAudio(fileName);
            break;
    }
    if (m_status == Status::CHROMAGRAM_ORIGINAL_READY) {
        discretizeChromagram();
    }
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
            if (n) n--;
            while (n) {
                pitchClassSequence.push_back(PitchClass(pc));
                n--;
            }
        }
    }
    return pitchClassSequence;
}

void Chromagram::printChromagram() {
    if (m_status != Status::CHROMAGRAM_ORIGINAL_READY &&
        m_status != Status::CHROMAGRAM_DISCRETE_READY) {
        return;
    }
    for (ChromagramMap::const_iterator itChr = m_discreteChromagramMap.begin();
         itChr != m_discreteChromagramMap.end(); itChr++) {
        double timestamp = itChr->first;
        ChromagramVector chrVector = itChr->second;
        std::cout << timestamp;
        for (ChromagramVector::const_iterator itChrVector = chrVector.begin();
            itChrVector != chrVector.end(); itChrVector++) {
            std::cout << "," << (*itChrVector);
        }
        std::cout << std::endl;
    }
}

void Chromagram::printOriginalChromagram(bool startOnANatural=true) {
    if (m_status != Status::CHROMAGRAM_ORIGINAL_READY &&
        m_status != Status::CHROMAGRAM_DISCRETE_READY) {
        return;
    }
    for (ChromagramMap::const_iterator itChr = m_originalChromagramMap.begin();
            itChr != m_originalChromagramMap.end(); itChr++) {
        double timestamp = itChr->first;
        ChromagramVector chrVector = itChr->second;
        std::cout << timestamp;
        for (int i = 0; i < chrVector.size(); i++) {
            int index = i;
            if (startOnANatural) {
                index = (PitchClass::PITCHCLASS_A_NATURAL + i) % PitchClass::NUMBER_OF_PITCHCLASSES;
            }
            std::cout << "," << chrVector[index];
        }
        std::cout << std::endl;
    }
}

void Chromagram::getChromagramFromCsv(std::string csvFilename) {
    std::ifstream infile;
    infile.open(csvFilename);
    if (!infile.is_open()) {
        m_status = Status::CHROMAGRAM_INPUTFILE_ERROR;
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
    m_status = Status::CHROMAGRAM_ORIGINAL_READY;
    return;
}

void Chromagram::getChromagramFromAudio(std::string audioFile) {
    SF_INFO sfinfo;
    SNDFILE *sndfile = sf_open(audioFile.c_str(), SFM_READ, &sfinfo);
    if (!sndfile) {
        m_status = Status::CHROMAGRAM_INPUTFILE_ERROR;
        return;
    }
    NNLSChroma *chroma = new NNLSChroma(sfinfo.samplerate);
    Vamp::HostExt::PluginInputDomainAdapter *ia =
    new Vamp::HostExt::PluginInputDomainAdapter(chroma);
    ia->setProcessTimestampMethod(
        Vamp::HostExt::PluginInputDomainAdapter::ShiftData);
    Vamp::HostExt::PluginBufferingAdapter *adapter =
    new Vamp::HostExt::PluginBufferingAdapter(ia);

    int blocksize = adapter->getPreferredBlockSize();

    if (!adapter->initialise(1, blocksize, blocksize)) {
        m_status = Status::CHROMAGRAM_NNLS_ERROR;
        return;
    }

    float *filebuf = new float[sfinfo.channels * blocksize];
    float *mixbuf = new float[blocksize];

    Vamp::Plugin::FeatureList chromaFeatures;
    Vamp::Plugin::FeatureSet fs;

    int chromaFeatureNo = -1;
    Vamp::Plugin::OutputList outputs = adapter->getOutputDescriptors();
    for (int i = 0; i < static_cast<int>(outputs.size()); ++i) {
        if (outputs[i].identifier == "chroma") {
            chromaFeatureNo = i;
        }
    }
    if (chromaFeatureNo < 0) {
        m_status = Status::CHROMAGRAM_NNLS_ERROR;
        return;
    }

    int frame = 0;
    while (frame < sfinfo.frames) {
        int count = -1;
        if ((count = sf_readf_float(sndfile, filebuf, blocksize)) <= 0) break;

        for (int i = 0; i < blocksize; ++i) {
            mixbuf[i] = 0.f;
            if (i < count) {
                for (int c = 0; c < sfinfo.channels; ++c) {
                    mixbuf[i] +=
                        filebuf[i * sfinfo.channels + c] / sfinfo.channels;
                }
            }
        }

        Vamp::RealTime timestamp =
            Vamp::RealTime::frame2RealTime(frame, sfinfo.samplerate);

        fs = adapter->process(&mixbuf, timestamp);

        chromaFeatures.insert(chromaFeatures.end(),
            fs[chromaFeatureNo].begin(),
            fs[chromaFeatureNo].end());

        frame += count;
    }

    sf_close(sndfile);

    fs = adapter->getRemainingFeatures();

    chromaFeatures.insert(chromaFeatures.end(),
        fs[chromaFeatureNo].begin(),
        fs[chromaFeatureNo].end());
    int pcIndex;
    double timestamp;
    for (int i = 0; i < static_cast<int>(chromaFeatures.size()); ++i) {
        timestamp = std::stof(chromaFeatures[i].timestamp.toString());
        for (int c = 0; c < chromaFeatures[i].values.size(); c++) {
            pcIndex =
                (PitchClass::PITCHCLASS_A_NATURAL + c)
                % PitchClass::NUMBER_OF_PITCHCLASSES;
            m_originalChromagramMap[timestamp][pcIndex] =
                chromaFeatures[i].values[c];
        }
    }

    delete[] filebuf;
    delete[] mixbuf;

    delete adapter;
    m_status = Status::CHROMAGRAM_ORIGINAL_READY;
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
    m_status = Status::CHROMAGRAM_DISCRETE_READY;
    return;
}

int Chromagram::getStatus() const {
    return m_status;
}

}  // namespace justkeydding

