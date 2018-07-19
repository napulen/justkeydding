/*
MIT License

Copyright (c) 2018 Nestor Napoles

Base class for pitch-class and key definitions

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

#ifndef INCLUDE_KEY_H_
#define INCLUDE_KEY_H_

#include<map>
#include<string>
#include<array>

namespace justkeydding {

class Key {
 public:
    explicit Key(std::string key);
    explicit Key(int key);
    int getInt() const;
    std::string getString() const;
    bool operator==(const Key &key) const;
    bool operator!=(const Key &key) const;
    enum enKey {
        // Major keys
        KEY_B_SHARP_MAJOR,
        KEY_C_NATURAL_MAJOR = 0,
        KEY_C_SHARP_MAJOR,
        KEY_D_FLAT_MAJOR = 1,
        KEY_D_NATURAL_MAJOR,
        KEY_D_SHARP_MAJOR,
        KEY_E_FLAT_MAJOR = 3,
        KEY_E_NATURAL_MAJOR,
        KEY_F_FLAT_MAJOR = 4,
        KEY_E_SHARP_MAJOR,
        KEY_F_NATURAL_MAJOR = 5,
        KEY_F_SHARP_MAJOR,
        KEY_G_FLAT_MAJOR = 6,
        KEY_G_NATURAL_MAJOR,
        KEY_G_SHARP_MAJOR,
        KEY_A_FLAT_MAJOR = 8,
        KEY_A_NATURAL_MAJOR,
        KEY_A_SHARP_MAJOR,
        KEY_B_FLAT_MAJOR = 10,
        KEY_B_NATURAL_MAJOR,
        KEY_C_FLAT_MAJOR = 11,
        // Minor keys
        KEY_B_SHARP_MINOR,
        KEY_C_NATURAL_MINOR = 12,
        KEY_C_SHARP_MINOR,
        KEY_D_FLAT_MINOR = 13,
        KEY_D_NATURAL_MINOR,
        KEY_D_SHARP_MINOR,
        KEY_E_FLAT_MINOR = 15,
        KEY_E_NATURAL_MINOR,
        KEY_F_FLAT_MINOR = 16,
        KEY_E_SHARP_MINOR,
        KEY_F_NATURAL_MINOR = 17,
        KEY_F_SHARP_MINOR,
        KEY_G_FLAT_MINOR = 18,
        KEY_G_NATURAL_MINOR,
        KEY_G_SHARP_MINOR,
        KEY_A_FLAT_MINOR = 20,
        KEY_A_NATURAL_MINOR,
        KEY_A_SHARP_MINOR,
        KEY_B_FLAT_MINOR = 22,
        KEY_B_NATURAL_MINOR,
        KEY_C_FLAT_MINOR = 23,
        NUMBER_OF_KEYS
    };

 private:
    int m_key;
    std::string m_keyString;
    std::map<int, std::string> m_intToStringMap;
    std::map<std::string, int> m_stringToIntMap;
    void initKeyMaps();
    void initIntToStringMap();
    void initStringToIntMap();
};

}  // namespace justkeydding

#endif  // INCLUDE_KEY_H_
