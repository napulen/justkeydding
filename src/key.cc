/*
MIT License

Copyright (c) 2018 Nestor Napoles

Key definitions

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

#include<map>
#include<string>
#include<array>
#include<algorithm>
#include<iostream>

#include "./key.h"

namespace justkeydding {

Key::Key(std::string keyString) {
    initKeyMaps();
    if (m_stringToIntMap.find(keyString) != m_stringToIntMap.end()) {
        m_key = m_stringToIntMap.at(keyString);
        m_keyString = keyString;
    }
}

Key::Key(int key) {
    initKeyMaps();
    if (key >= 0 && key < NUMBER_OF_KEYS) {
        m_key = key;
        m_keyString = m_intToStringMap.at(m_key);
    }
}

void Key::initKeyMaps() {
    initIntToStringMap();
    initStringToIntMap();
}

void Key::initIntToStringMap() {
    m_intToStringMap[KEY_C_NATURAL_MAJOR] = "C";
    m_intToStringMap[KEY_D_FLAT_MAJOR] = "Db";
    m_intToStringMap[KEY_D_NATURAL_MAJOR] = "D";
    m_intToStringMap[KEY_E_FLAT_MAJOR] = "Eb";
    m_intToStringMap[KEY_E_NATURAL_MAJOR] = "E";
    m_intToStringMap[KEY_F_NATURAL_MAJOR] = "F";
    m_intToStringMap[KEY_F_SHARP_MAJOR] = "F#";
    m_intToStringMap[KEY_G_NATURAL_MAJOR] = "G";
    m_intToStringMap[KEY_A_FLAT_MAJOR] = "Ab";
    m_intToStringMap[KEY_A_NATURAL_MAJOR] = "A";
    m_intToStringMap[KEY_B_FLAT_MAJOR] = "Bb";
    m_intToStringMap[KEY_B_NATURAL_MAJOR] = "B";
    m_intToStringMap[KEY_C_NATURAL_MINOR] = "c";
    m_intToStringMap[KEY_C_SHARP_MINOR] = "c#";
    m_intToStringMap[KEY_D_NATURAL_MINOR] = "d";
    m_intToStringMap[KEY_E_FLAT_MINOR] = "eb";
    m_intToStringMap[KEY_E_NATURAL_MINOR] = "e";
    m_intToStringMap[KEY_F_NATURAL_MINOR] = "f";
    m_intToStringMap[KEY_F_SHARP_MINOR] = "f#";
    m_intToStringMap[KEY_G_NATURAL_MINOR] = "g";
    m_intToStringMap[KEY_A_FLAT_MINOR] = "ab";
    m_intToStringMap[KEY_A_NATURAL_MINOR] = "a";
    m_intToStringMap[KEY_B_FLAT_MINOR] = "bb";
    m_intToStringMap[KEY_B_NATURAL_MINOR] = "b";
}

void Key::initStringToIntMap() {
    m_stringToIntMap["B#"] = KEY_B_SHARP_MAJOR;
    m_stringToIntMap["C"] = KEY_C_NATURAL_MAJOR;
    m_stringToIntMap["C#"] = KEY_C_SHARP_MAJOR;
    m_stringToIntMap["Db"] = KEY_D_FLAT_MAJOR;
    m_stringToIntMap["D"] = KEY_D_NATURAL_MAJOR;
    m_stringToIntMap["D#"] = KEY_D_SHARP_MAJOR;
    m_stringToIntMap["Eb"] = KEY_E_FLAT_MAJOR;
    m_stringToIntMap["E"] = KEY_E_NATURAL_MAJOR;
    m_stringToIntMap["Fb"] = KEY_F_FLAT_MAJOR;
    m_stringToIntMap["E#"] = KEY_E_SHARP_MAJOR;
    m_stringToIntMap["F"] = KEY_F_NATURAL_MAJOR;
    m_stringToIntMap["F#"] = KEY_F_SHARP_MAJOR;
    m_stringToIntMap["Gb"] = KEY_G_FLAT_MAJOR;
    m_stringToIntMap["G"] = KEY_G_NATURAL_MAJOR;
    m_stringToIntMap["G#"] = KEY_G_SHARP_MAJOR;
    m_stringToIntMap["Ab"] = KEY_A_FLAT_MAJOR;
    m_stringToIntMap["A"] = KEY_A_NATURAL_MAJOR;
    m_stringToIntMap["A#"] = KEY_A_SHARP_MAJOR;
    m_stringToIntMap["Bb"] = KEY_B_FLAT_MAJOR;
    m_stringToIntMap["B"] = KEY_B_NATURAL_MAJOR;
    m_stringToIntMap["Cb"] = KEY_C_FLAT_MAJOR;
    m_stringToIntMap["b#"] = KEY_B_SHARP_MINOR;
    m_stringToIntMap["c"] = KEY_C_NATURAL_MINOR;
    m_stringToIntMap["c#"] = KEY_C_SHARP_MINOR;
    m_stringToIntMap["db"] = KEY_D_FLAT_MINOR;
    m_stringToIntMap["d"] = KEY_D_NATURAL_MINOR;
    m_stringToIntMap["d#"] = KEY_D_SHARP_MINOR;
    m_stringToIntMap["eb"] = KEY_E_FLAT_MINOR;
    m_stringToIntMap["e"] = KEY_E_NATURAL_MINOR;
    m_stringToIntMap["fb"] = KEY_F_FLAT_MINOR;
    m_stringToIntMap["e#"] = KEY_E_SHARP_MINOR;
    m_stringToIntMap["f"] = KEY_F_NATURAL_MINOR;
    m_stringToIntMap["f#"] = KEY_F_SHARP_MINOR;
    m_stringToIntMap["gb"] = KEY_G_FLAT_MINOR;
    m_stringToIntMap["g"] = KEY_G_NATURAL_MINOR;
    m_stringToIntMap["g#"] = KEY_G_SHARP_MINOR;
    m_stringToIntMap["ab"] = KEY_A_FLAT_MINOR;
    m_stringToIntMap["a"] = KEY_A_NATURAL_MINOR;
    m_stringToIntMap["a#"] = KEY_A_SHARP_MINOR;
    m_stringToIntMap["bb"] = KEY_B_FLAT_MINOR;
    m_stringToIntMap["b"] = KEY_B_NATURAL_MINOR;
    m_stringToIntMap["cb"] = KEY_C_FLAT_MINOR;
}

int Key::getInt() const {
    return m_key;
}

std::string Key::getString() const {
    return m_keyString;
}

bool Key::operator==(const Key &key) const {
    return m_key == key.getInt();
}

bool Key::operator!=(const Key &key) const {
    return m_key != key.getInt();
}

}  // namespace justkeydding
