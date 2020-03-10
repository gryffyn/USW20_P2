//
// Created by gryffyn on 18/02/2020.
//<

#include "User.h"

#include <iostream>
#include <utility>
#include "ObjStore.h"

#include "Key.h"


User::User(int id, const std::string& name, const std::string& user, std::string unhashed){
    ObjStore db;
    usertype = "Student";
    std::stringstream ss;
    ss << "INSERT INTO Users(user_id, user_type, name, user, pwhash) VALUES (" ;
    ss << id << ", '" << usertype << "', '" << name << "', '" << user << "', '" << hashpw(unhashed) << "');";
    std::string str = ss.str();
    db.insert(ss.str());
}

std::string User::hashpw(std::string& unhashed) {
    try {
        Key key(unhashed);
        return key.get_key();
    } catch (std::exception& e) {
    }
}
