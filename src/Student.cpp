//
// Created by gryffyn on 18/02/2020.
//

#include "Student.hpp"

#include <mariadb++/connection.hpp>

#include "ObjStore.hpp"

using namespace mariadb;

Student::Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute("INSERT INTO Students(user_id) VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1))");
}

Student::Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash,
                 std::string data)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute(
        "INSERT INTO Students(user_id, data) "
        "VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1), '" + data + "');");
}

void Student::save_data(unsigned int userid, std::string data) {
    std::stringstream ss;
    ss << "UPDATE Students SET data = '" << data << "' WHERE user_id = '" << userid << "';";
    db.execute(ss.str());
}

std::string Student::get_data(unsigned int userid) {
    std::stringstream ss;
    ss << "SELECT user_id, data FROM Students WHERE user_id = '" << userid << "';";
    result_set_ref result = db.select(ss.str());
    return result->get_string(1);
}
