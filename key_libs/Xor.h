// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#ifndef USW20_P2_XOR_H
#define USW20_P2_XOR_H

#include <string>

class Xor {
public:
    void xorString(std::string plain, std::string key);
    std::string getXoredText();
private:
    std::string xorKey;
};


#endif //USW20_P2_XOR_H
