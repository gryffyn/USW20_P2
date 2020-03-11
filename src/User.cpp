//
// Created by gryffyn on 18/02/2020.
//<

#include "User.hpp"

#include <iostream>
#include <mariadb++/connection.hpp>
#include <utility>

#include "Key.hpp"
#include "ObjStore.hpp"

User::User(ObjStore& db, int id, const std::string& name, const std::string& user, std::string unhashed){
    std::stringstream ss;
    ss << "INSERT INTO Users(user_id, name, user, pwhash) VALUES (" ;
    ss << id << ", '" << name << "', '" << user << "', '" << hashpw(unhashed) << "');";
    db.insert(ss.str());
}

User::User(ObjStore& db, const std::string& name, const std::string& user, std::string unhashed){
    std::stringstream ss;
    ss << "INSERT INTO Users(name, user, pwhash) VALUES (" ;
    ss << "'" << name << "', '" << user << "', '" << hashpw(unhashed) << "');";
    db.insert(ss.str());
}

std::string User::hashpw(std::string& unhashed) {
    try {
        Key key(unhashed);
        return key.get_key();
    } catch (std::exception& e) {
        return (e.what());
    }
}


