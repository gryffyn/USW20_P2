//
// Created by gryffyn on 16/03/2020.
//

#include "Admin.hpp"

Admin::Admin(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute("INSERT INTO Admins(user_id) VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1))");
}