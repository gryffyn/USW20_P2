//
// Created by gryffyn on 18/02/2020.
//

#include "User.h"

#include <iostream>
#include <utility>

#include "Key.h"


User::User(std::string id, const std::string& name, std::string user, std::string age; std::string unhashed){
    user_details.id = id;
    user_details.user_name = name;
    user_details.user_user = user;
    user_details.age = age;
}

std::string User::hashpw(std::string& unhashed) {
    try {
        Key key(unhashed);
        return key.get_key();
    } catch (std::exception& e) {
    }
}

User::user_details_s User::get_user() {
    return user_details;
}