//
// Created by gryffyn on 18/02/2020.
//

#include "User.h"

#include <iostream>
#include <utility>

#include "Key.h"

User::User(char id[8], const std::string& name, std::string user, std::string unhashed){
}

std::string User::hashpw(std::string& unhashed) {
    try {
        Key key(unhashed);
        return key.get_key();
    } catch (std::exception& e) {
    }
}
