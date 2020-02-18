// Copyright 2018 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include "Xor.h"
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>

std::vector<unsigned char> stringToCharVector(std::string str){
    std::vector<unsigned char> cVector;
    for (char& i : str) {
        std::bitset<8> bChar(i);
        unsigned long ulChar = bChar.to_ulong();
        cVector.push_back(static_cast<char>(ulChar));
    }
    return cVector;
}

std::vector<unsigned char> xorPlain(std::vector<unsigned char> key, std::vector<unsigned char> plaintext) { // xors plaintext with key
    std::vector<unsigned char> output(plaintext); // creates output vector
    for(auto i = 0ull; i < plaintext.size(); i++) { //iterates through the plaintext vector
        output[i] = plaintext[i] ^ key[i % (key.size() / sizeof(char))]; // xors each char with each char of the key
    }
    return output;
}

void Xor::xorString(std::string plain, std::string key) { // xors test
    std::vector<unsigned char> xoredtext = xorPlain(stringToCharVector(key),stringToCharVector(plain));
    std::stringstream ss;
    for(unsigned char& i : xoredtext) {
        ss << i;
    }
    xorKey = ss.str();
}

std::string Xor::getXoredText() { // getter for key
    return xorKey;
}
