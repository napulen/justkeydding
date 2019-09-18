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

#include "./justkeydding.h"

using justkeydding::PitchClass;
using justkeydding::Key;
using justkeydding::KeyProfile;
using justkeydding::KeyTransition;
using justkeydding::HiddenMarkovModel;
using justkeydding::Chromagram;
using justkeydding::Status;
using justkeydding::Midi;

int main(int argc, char *argv[]) {
    optparse::OptionParserExcept parser;
    initOptionParser(&parser);
    justkeydding::enInputType inputType;
    std::string keyTransition;
    std::string majorKeyProfile;
    std::string minorKeyProfile;
    KeyProfile::KeyProfileArray majorCustomKeyProfile;
    KeyProfile::KeyProfileArray minorCustomKeyProfile;
    KeyTransition::KeyTransitionArray customKeyTransition;
    std::string filename;
    int status;
    bool justEvaluation;
    bool justProbabilities;
    bool chromaOnly;
    try {
        const optparse::Values &options = parser.parse_args(argc, argv);
        const std::vector<std::string> args = parser.args();
        if (args.size() != 1) {
            std::cout << "Missing input file." << std::endl;
            parser.print_help();
            return 0;
        }
        filename = args.front();
        if (options.is_set("inputformat")) {
            // What kind of file are we reading
            std::string format = static_cast<std::string>(
                options.get("inputformat"));
            if (format == ".wav") {
                inputType = justkeydding::INPUT_WAV;
            } else if (format == ".csv") {
                inputType = justkeydding::INPUT_CSV;
            } else if (format == ".midi") {
                inputType = justkeydding::INPUT_MIDI;
            }
        } else {
            // The user did not provide a file format, let's find out
            std::string extension =
                filename.substr(filename.find_last_of("."));
            std::transform(
                extension.begin(),
                extension.end(),
                extension.begin(),
                ::tolower);
            if (extension == ".wav") {
                inputType = justkeydding::INPUT_WAV;
            } else if (extension == ".csv") {
                inputType = justkeydding::INPUT_CSV;
            } else if (extension == ".midi" || extension == ".mid") {
                inputType = justkeydding::INPUT_MIDI;
            } else {
                std::cout << "It seems the input is neither a wav, csv, nor midi file. If it is, please specify explicitly using -f." << std::endl;
                parser.print_help();
                return 0;
            }
        }
        if (options.is_set("customkeyprofiles")) {
            // The user wants to define custom key profiles
            std::string arrayStr = static_cast<std::string>(
                options.get("customkeyprofiles"));
            majorKeyProfile = "custom";
            minorKeyProfile = "custom";
            std::stringstream arrayStream(arrayStr);
            double val = 0;
            int i = 0;
            while (arrayStream >> val) {
                if (i < Key::FIRST_MINOR_KEY) {
                    majorCustomKeyProfile[i] = val;
                } else{
                    int pitchClasses = PitchClass::NUMBER_OF_PITCHCLASSES;
                    minorCustomKeyProfile[i % pitchClasses] = val;
                }
                i++;
            }
            if (i != Key::NUMBER_OF_KEYS) {
                std::cout << "You must provide exactly 24 values for the custom key profiles. You provided " << i << "." << std::endl;
                parser.print_help();
                return 0;
            }
        } else {
            // The user wants one of the predefined key profiles
            majorKeyProfile = static_cast<std::string>(
                options.get("majorprofile"));
            minorKeyProfile = static_cast<std::string>(
                options.get("minorprofile"));
        }
        if (options.is_set("customkeytransitions")) {
            // The user wants to define custom key transitions
            std::string arrayStr = static_cast<std::string>(
                options.get("customkeytransitions"));
            keyTransition = "custom";
            std::stringstream arrayStream(arrayStr);
            double val = 0;
            int i = 0;
            while (arrayStream >> val) {
                customKeyTransition[i] = val;
                i++;
            }
            if (i != Key::NUMBER_OF_KEYS) {
                std::cout << "You must provide exactly 24 values for the custom key transitions. You provided " << i << "." << std::endl;
                parser.print_help();
                return 0;
            }
        }
        else {
            // The user wants one of the predefined key transitions
            keyTransition = static_cast<std::string>(
                options.get("keytransition"));
        }
        justEvaluation = options.is_set_by_user("evaluate");
        justProbabilities = options.is_set_by_user("probabilities");
        chromaOnly = options.is_set_by_user("chromaonly");
    }
    catch (int ret_code) {
        std::cerr << "Error " << ret_code << std::endl;
        return ret_code;
    }
    // Receiving MIDI input
    PitchClass::PitchClassSequence pitchClassSequence;
    if (inputType == justkeydding::INPUT_MIDI) {
        Midi midi = Midi(filename);
        if  ((status = midi.getStatus()) != Status::MIDI_READY) {
            std::cerr << "There was an error while"
                    " reading the input file." << std::endl;
            return status;
        }
        pitchClassSequence = midi.getPitchClassSequence();
    }
    // Receiving Audio (WAV or CSV) input
    else {
        Chromagram::enFileType fileType;
        if (inputType == justkeydding::INPUT_CSV) {
            fileType = Chromagram::FILETYPE_CSV;
        } else if (inputType == justkeydding::INPUT_WAV) {
            fileType = Chromagram::FILETYPE_AUDIO;
        }
        // Get the chromagrams
        Chromagram chr = Chromagram(filename, fileType);
        if ((status = chr.getStatus()) != Status::CHROMAGRAM_DISCRETE_READY) {
            std::cerr << "There was an error while"
                        " reading the input file." << std::endl;
            return status;
        }
        if (chromaOnly) {
            bool startOnANatural = true;
            chr.printOriginalChromagram(startOnANatural);
            return 0;
        }
        // Turn into a PitchcClassSequence
        pitchClassSequence = chr.getPitchClassSequence();
    }
    // States section
    Key::KeyVector keyVector =
        Key::getAllKeysVector();
    // Initial probabilities
    KeyTransition::KeyTransitionArray symmetrical =
        KeyTransition("symmetrical").getKeyTransitionArray();
    std::map<Key, double> initialProbabilities;
    for (Key::KeyVector::iterator it = keyVector.begin();
        it != keyVector.end(); it++) {
        initialProbabilities[*it] = symmetrical[it->getInt()];
    }
    // Transition probabilities
    KeyTransition::KeyTransitionMap transitionProbabilities =
        KeyTransition(
            keyTransition,
            customKeyTransition).getKeyTransitionMap();
    // Emission probabilities
    KeyProfile::KeyProfileMap emissionProbabilities =
        KeyProfile(
            majorKeyProfile,
            minorKeyProfile,
            majorCustomKeyProfile,
            minorCustomKeyProfile).getKeyProfileMap();
    Key::KeySequence keySequence;
    double maximumProbability;
    HiddenMarkovModel hmm(
        pitchClassSequence,
        keyVector,
        initialProbabilities,
        transitionProbabilities,
        emissionProbabilities);
    hmm.runViterbi();
    if ((status = hmm.getStatus()) !=
        Status::HIDDENMARKOVMODEL_VITERBI_READY) {
        std::cerr << "There was an error while"
                    " running the model." << std::endl;
        return status;
    }
    keySequence = hmm.getKeySequence();
    /////////////////////////////
    // Second Hidden Markov Model
    /////////////////////////////
    KeyTransition::KeyTransitionMap zeroTransiionProbabilities =
        KeyTransition("zero").getKeyTransitionMap();
    HiddenMarkovModel hmm2(
        keySequence,
        keyVector,
        initialProbabilities,
        zeroTransiionProbabilities,
        transitionProbabilities);
    hmm2.runViterbi();
    keySequence = hmm2.getKeySequence();
    HiddenMarkovModel::ProbabilityVector probabilityVector = hmm2.getProbabilityVector();
    Key mainKey = keySequence.front();
    if (justProbabilities) {
        for (HiddenMarkovModel::ProbabilityVector::const_iterator itKeyProb =
        probabilityVector.begin(); itKeyProb != probabilityVector.end();
        itKeyProb++) {
            std::cout << *itKeyProb << " ";
        }
        std::cout << std::endl;
    }
    else if (justEvaluation) {
        std::size_t underscore = filename.find_last_of("_");
        std::size_t dot = filename.find_last_of(".");
        if (underscore != std::string::npos && dot != std::string::npos) {
            underscore++;
            std::string groundTruthString =
                filename.substr(underscore, dot - underscore);
            Key groundTruth = Key(groundTruthString);
            double score = 0.0;
            if (mainKey == groundTruth) {
                score = 1.0;
            } else if (mainKey == groundTruth.getDominantKey()) {
                score = 0.5;
            } else if (mainKey == groundTruth.getRelativeKey()) {
                score = 0.3;
            } else if (mainKey == groundTruth.getParallelKey()) {
                score = 0.2;
            }
            std::cout << score << std::endl;
        }
    }
    else {
        std::string mainKeyStr = mainKey.getString();
        std::transform(
            mainKeyStr.begin(),
            std::next(mainKeyStr.begin()),
            mainKeyStr.begin(),
            ::toupper);
        std::cout
            << mainKeyStr << '\t'
            << (mainKey.isMajorKey() ? "major" : "minor")
            << std::endl;
    }
    return 0;
}

void initOptionParser(optparse::OptionParserExcept *parser) {
    const std::string usage =
        "usage: %prog [options] inputfile";

    const std::string version =
        "%prog v0.4.0\nCopyright (C) Nestor Napoles 2018.";

    (*parser)
        .usage(usage)
        .version(version)
        .description(
            "Audio Key Detection program based on NNLS-Chroma"
            " features and a Hidden Markov Model");

    std::array<std::string, 3> formatOptions =
        {"wav", "csv", "midi"};
    (*parser).add_option("-f", "--inputformat")
        .choices(formatOptions.begin(), formatOptions.end())
        .help("Type of input")
        .metavar("wav|csv|midi");

    std::array<std::string, 5> majorKeyProfiles =
        {"krumhansl_kessler", "aarden_essen",
        "bellman_budge",  "sapp", "temperley"};
    (*parser).add_option("-M", "--majorprofile")
        .choices(majorKeyProfiles.begin(), majorKeyProfiles.end())
        .set_default("sapp")
        .help("Major key profiles");

    std::array<std::string, 5> minorKeyProfiles =
        {"krumhansl_kessler", "aarden_essen",
        "bellman_budge",  "sapp", "temperley"};
    (*parser).add_option("-m", "--minorprofile")
        .choices(minorKeyProfiles.begin(), minorKeyProfiles.end())
        .set_default("sapp")
        .help("Minor key profiles");

    (*parser).add_option("-K", "--customkeyprofiles")
        .help("Provide your own key profiles")
        .metavar("Array[24]");

    std::array<std::string, 2> keyTransitions =
        {"exponential2", "exponential10"};
    (*parser).add_option("-t", "--keytransition")
        .choices(keyTransitions.begin(), keyTransitions.end())
        .set_default("exponential10")
        .help("Key transition profiles");

    (*parser).add_option("-T", "--customkeytransitions")
        .help("Provide your own key transitions")
        .metavar("Array[24]");

    (*parser).add_option("-e", "--evaluate")
        .action("store_true");

    (*parser).add_option("-p", "--probabilities")
        .action("store_true");
    
    (*parser).add_option("-c", "--chromaonly")
        .action("store_true");
}

