// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include "Key.hpp"

#include <sodium.h>

#include <sstream>
#include <string>

class hash_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override { return "Hash func ran out of memory"; }
};

class Key::pass_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override { return "Password is invalid length/strength."; }
};

enum passStrength{Strong, Medium, Weak, Invalid}; // emum containing options for password strength
enum chartype{upper, digit, symbol};

int CountInString(chartype type, std::string input){ // counts the number of occurences of a type of char in the string
    int count = 0; // initializes count to 0 in case there are no occurences
    if(type == upper) {
        for (char c : input){
            if(isupper(c)){++count;}
        }
    } else if (type == digit) {
        for (char c : input){
            if(isdigit(c)){++count;}
        }
    } else if (type == symbol){
        for (char c : input) {
            if (!(isalnum(c))){++count;}
        }
    }
    return count;
}

passStrength CheckPasswordStrength(std::string& password){ // checks pw strength and returns an enum
    if ((password.length() >= 18) && (CountInString(upper,password) >= 4) && (CountInString(digit,password) >= 3) && (CountInString(symbol,password) >= 3)){
        return Strong;
    } else if ((password.length() >= 16) && (CountInString(upper,password) >= 2) && (CountInString(digit,password) >= 2) && (CountInString(symbol,password) >= 2)) {
        return Medium;
    } else if ((password.length() >= 14) && (CountInString(upper,password) >= 1) && (CountInString(digit,password) >= 1) && (CountInString(symbol,password) >= 1)) {
        return Weak;
    } else { // if it fails all other reqs, it must be invalid
        return Invalid;
    }
}

Key::Key(std::string pass) {
    // locks memory from the mem address of the start of pass to the end of the str
    sodium_mlock((void*)pass.c_str(), pass.size());
    if (!(CheckPasswordStrength(pass) == Invalid)){
        throw pass_exception();
    } else {
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
