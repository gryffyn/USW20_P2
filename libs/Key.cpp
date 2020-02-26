// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <string>
#include "Key.h"
#include <sstream>
#include <sodium.h>
#include <sqlite3.h>

class hash_exception : public std::exception {
    [[nodiscard]] const char * what () const noexcept override{
        return "Hash func ran out of memory";
    }
};

struct key_salt{

};

Key::Key(const std::string& pass) {
    char hashed_password[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(hashed_password, pass.c_str(), pass.length(), crypto_pwhash_OPSLIMIT_SENSITIVE,
            crypto_pwhash_MEMLIMIT_SENSITIVE) != 0) {
        throw hash_exception();
    }
    std::stringstream ss;
    ss << hashed_password;
    finalkey = ss.str();
}

std::string Key::get_key() {
    return finalkey;
}
