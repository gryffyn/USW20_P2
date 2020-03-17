//
// Created by gryffyn on 17/03/2020.
//

#ifndef USW20_P2_XOR_HPP
#define USW20_P2_XOR_HP

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

namespace Xor {

vector<char> string_to_vector_char(string str) {
    vector<char> cVector;
    for (size_t i = 0; i < str.size(); i++) {
        bitset<8> bChar(str[i]);
        unsigned long ulChar = bChar.to_ulong();
        cVector.push_back(static_cast<char>(ulChar));
    }
    return cVector;
}

string xor_string(string& s_key, string& text) {  // https://github.com/KyleBanks/XOREncryption/blob/master/C++/main.cpp
    vector<char> key = string_to_vector_char(s_key);
    vector<char> plaintext = string_to_vector_char(text);
    vector<char> output = plaintext;              // creates output vector
    for (size_t i = 0; i < plaintext.size(); i++) {                       // iterates through the plaintext vector
        output[i] = plaintext[i] ^ key[i % (key.size() / sizeof(char))];  //
    }
    string final(output.begin(), output.end());
    return final;
}

}  // namespace Xor

#endif  // USW20_P2_XOR_HPP
