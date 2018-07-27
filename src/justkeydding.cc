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

int main(int argc, char *argv[]) {
    const std::string usage =
        "usage: %prog [options] inputfile";

    const std::string version =
        "%prog v0.4.0\nCopyright (C) Nestor Napoles 2018.";

    optparse::OptionParserExcept parser;
    parser
        .usage(usage)
        .version(version)
        .description(
            "Audio Key Detection program based on NNLS-Chroma"
            " features and a Hidden Markov Model");

    std::array<std::string, 3> formatOptions =
        {"wav", "csv", "midi"};
    parser.add_option("-f", "--inputformat")
        .choices(formatOptions.begin(), formatOptions.end());
    parser.set_defaults("inputformat", "wav");

    std::array<std::string, 3> majorKeyProfiles =
        {"krumhansl_kessler", "sapp", "temperley"};
    parser.add_option("-M", "--majorprofile")
        .choices(majorKeyProfiles.begin(), majorKeyProfiles.end());
    parser.set_defaults("majorprofile", "temperley");

    std::array<std::string, 3> minorKeyProfiles =
        {"krumhansl_kessler", "sapp", "temperley"};
    parser.add_option("-m", "--minorprofile")
        .choices(minorKeyProfiles.begin(), minorKeyProfiles.end());
    parser.set_defaults("minorprofile", "sapp");

    std::array<std::string, 3> keyTransitions =
        {"exponential", "exponential10"};
    parser.add_option("-t", "--keytransition")
        .choices(keyTransitions.begin(), keyTransitions.end());
    parser.set_defaults("keytransition", "exponential10");

    Chromagram::enFileType fileformat;
    std::string keyTransition;
    std::string majorKeyProfile;
    std::string minorKeyProfile;
    std::string filename;

    try {
        const optparse::Values &options = parser.parse_args(argc, argv);
        const std::vector<std::string> args = parser.args();

        if (args.size() != 1) {
            std::cout << "Missing input file." << std::endl;
            parser.print_help();
            return 0;
        } else {
            if (options.is_set("inputformat")) {
                std::string format = static_cast<std::string>(
                    options.get("inputformat"));
                if (format == "wav") {
                    fileformat = Chromagram::FILETYPE_AUDIO;
                } else if (format == "csv") {
                    fileformat = Chromagram::FILETYPE_CSV;
                } else if (format == "midi") {
                    std:: cout << "We don't support midi yet!"
                        << std::endl;
                    parser.print_help();
                    return 0;
                }
            }
            if (options.is_set("majorprofile")) {
                majorKeyProfile = static_cast<std::string>(
                    options.get("majorprofile"));
            }
            if (options.is_set("minorprofile")) {
                minorKeyProfile = static_cast<std::string>(
                    options.get("minorprofile"));
            }
            if (options.is_set("keytransition")) {
                keyTransition = static_cast<std::string>(
                    options.get("keytransition"));
            }
            filename = args.front();
        }
    }
    catch (int ret_code) {
        std::cout << "OptionParser has thrown " << ret_code << std::endl;
        return ret_code;
    }

    // Get the chromagrams
    Chromagram chr = Chromagram(filename, fileformat);
    // Turn into a PitchcClassSequence
    PitchClass::PitchClassSequence pitchClassSequence;
    pitchClassSequence = chr.getPitchClassSequence();
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
        KeyTransition(keyTransition).getKeyTransitionMap();
    // Emission probabilities
    KeyProfile::KeyProfileMap emissionProbabilities =
        KeyProfile(majorKeyProfile, minorKeyProfile)
            .getKeyProfileMap();
    Key::KeySequence keySequence;
    double maximumProbability;
    HiddenMarkovModel hmm(
        pitchClassSequence,
        keyVector,
        initialProbabilities,
        transitionProbabilities,
        emissionProbabilities);
    hmm.runViterbi();
    keySequence = hmm.getKeySequence();
    maximumProbability = hmm.getMaximumProbability();
    // std::cout << "Maximum probability of the sequence: "
    //     << maximumProbability << std::endl;
    // for (Key::KeySequence::const_iterator itKey = keySequence.begin();
    //     itKey != keySequence.end(); itKey++) {
    //     std::cout << itKey->getString() << " ";
    // }
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
    maximumProbability = hmm2.getMaximumProbability();
    // std::cout << "Maximum probability of the sequence: "
    //     << maximumProbability << std::endl;
    // for (Key::KeySequence::const_iterator itKey = keySequence.begin();
    //     itKey != keySequence.end(); itKey++) {
    //     std::cout << itKey->getString() << " ";
    // }
    Key mainKey = keySequence.front();
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
    return 0;
}
