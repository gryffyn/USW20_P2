//
// Created by gryffyn on 18/02/2020.
//

#include "Student.hpp"

#include <sodium.h>
#include <mariadb++/connection.hpp>
#include <utility>
#include <string>

#include "ObjStore.hpp"
#include "DataTools.hpp"

using namespace mariadb;

Student::Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute("INSERT INTO Students(user_id) VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1))");
}

Student::Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash, std::string password,
                 std::string data)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    mariadb::result_set_ref result = db.select("SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1;");
    result->next();
    int user_id = result->get_unsigned16("user_id");
    std::stringstream ss;
    ss << "INSERT INTO Students(user_id) VALUES (" << user_id << ");";
    db.execute(ss.str());
    DataTools::save_data(db, user_id, "Student", data, password);
}

