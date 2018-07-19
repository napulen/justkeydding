/*
MIT License

Copyright (c) 2018 Nestor Napoles

Common key-profiles for key detection algorithms

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
#include<array>
#include<iostream>

#include "./keyprofile.h"

int main(int argc, char *argv[]) {
    // TODO(napulen): Complete some actual unit tests
    justkeydding::KeyProfile kp("sapp");
    justkeydding::KeyProfile::KeyProfileArray kpMajor;
    justkeydding::KeyProfile::KeyProfileArray kpMinor;
    kpMajor = kp.getMajorKeyProfile();
    kpMinor = kp.getMinorKeyProfile();
    for (int i=0; i < kpMajor.size(); i++) {
        std::cout << kp.whichMajorKeyProfile()
                << " " << kp.whichMinorKeyProfile()
                << " " << kpMajor[i]
                << " " << kpMinor[i] << std::endl;
    }
}
