// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <string>
#include "Key.h"
#include <sstream>

struct key_salt{

};

Key::Key(std::string pass) {
    std::stringstream ss;
    finalkey = ss.str();
}

std::string Key::get_key() {
    return finalkey;
}
