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

#include "./key.h"

namespace justkeydding {

Key::Key(std::string keyString) {
    initStringToIntMap();
    if (m_stringToIntMap.find(keyString) != m_stringToIntMap.end()) {
        m_key = m_stringToIntMap.at(keyString);
        m_keyString = keyString;
    }
}

Key::Key(int key) {
    initStringToIntMap();
    if (static_cast<unsigned int>(key) < NUMBER_OF_KEYS) {
        m_key = key;
        m_keyString = m_allKeyStrings[m_key];
    }
}

Key::KeyVector Key::getAllKeysVector() {
    KeyVector keyVector;
    for (int keyIndex = 0; keyIndex < NUMBER_OF_KEYS; keyIndex++) {
         keyVector.push_back(Key(keyIndex));
    }
    return keyVector;
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

bool Key::isMajorKey() const {
    return m_key >= FIRST_MAJOR_KEY &&
            m_key <= LAST_MAJOR_KEY;
}

bool Key::isMinorKey() const {
    return m_key >= FIRST_MINOR_KEY &&
            m_key <= LAST_MINOR_KEY;
}

bool Key::operator==(const Key &key) const {
    return m_key == key.getInt();
}

bool Key::operator!=(const Key &key) const {
    return m_key != key.getInt();
}

bool Key::operator<(const Key &key) const {
    return m_key < key.getInt();
}

Key& Key::operator=(const Key &key) {
    m_key = key.getInt();
    m_keyString = key.getString();
    return *this;
}

Key Key::getRelativeKey() const {
    int relative;
    if (this->isMajorKey()) {
        relative = ((m_key + 9) % 12) + 12;
    } else {
        relative = (m_key - 9) % 12;
    }
    return Key(relative);
}

Key Key::getParallelKey() const {
    int parallel = m_key;
    if (this->isMajorKey()) {
      parallel += 12;
    } else {
      parallel -= 12;
    }
    return Key(parallel);
}

Key Key::getDominantKey() const {
    int dominant = (m_key + 7) % 12;
    dominant += this->isMinorKey() ? 12 : 0;
    return Key(dominant);
}

Key Key::getSubDominantKey() const {
    int subdominant = (m_key + 5) % 12;
    subdominant += this->isMinorKey() ? 12 : 0;
    return Key(subdominant);
}

}  // namespace justkeydding
