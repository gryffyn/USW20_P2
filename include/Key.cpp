// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include "Key.h"

#include <sodium.h>

#include <sstream>
#include <string>

class hash_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override { return "Hash func ran out of memory"; }
};

Key::Key(const std::string& pass) {
    // locks memory from the mem address of the start of pass to the end of the str
    sodium_mlock((void*)pass.c_str(), pass.size());
    // creates char[] of required length for the hash
    char hashed_password[crypto_pwhash_STRBYTES];
    // hashes the password, throws on failed mem alloc
    if (crypto_pwhash_str(hashed_password, pass.c_str(), pass.length(), crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw hash_exception();
    }
    // unlocks and zeroes the memory
    sodium_munlock((void*)pass.c_str(), pass.size());
    // dumps hash into string and returns it
    std::stringstream ss;
    ss << hashed_password;
    finalkey = ss.str();
}

bool Key::verify_key(std::string key, std::string pass) {
    // locks memory from the mem address of the start of pass to the end of the str
    sodium_mlock((void*)pass.c_str(), pass.size());
    // verifies the password against the stored hash
    int ret_val = crypto_pwhash_str_verify(key.c_str(), pass.c_str(), pass.length());
    // unlocks and zeroes the memory
    sodium_munlock((void*)pass.c_str(), pass.size());
    // crypto_pwhash_str_verify returns 0 if correct, so return value needs to be inverted after cast to bool
    return !ret_val;
}

std::string Key::get_key() { return finalkey; }
