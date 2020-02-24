// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <string>
#include "Key.h"
#include <random>
#include "Xor.h"

std::string Key::gen_key(){
    std::random_device rand;
    auto prng = std::ranlux48_base(rand.operator()());
    unsigned long long int key = prng.operator()();
    return std::to_string(key);
}

Key::Key(std::string pass) {
    std::string key_init = gen_key();
    Xor xor_engine;
    xor_engine.xorString(key_init, pass);
    finalkey = xor_engine.getXoredText();
}

std::string Key::get_key() {
    return finalkey;
}
