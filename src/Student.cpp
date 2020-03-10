//
// Created by gryffyn on 18/02/2020.
//

#include "Student.hpp"

#include <utility>
#include "ObjStore.hpp"
#include <mariadb++/connection.hpp>

using namespace mariadb;

Student::Student(int id, const std::string& name, const std::string& user, std::string unhashed)
    : User(id, name, user, std::move(unhashed)) {

}

void Student::save_data(unsigned int userid, std::string data) {
    ObjStore db;
    std::stringstream ss;
    ss << "UPDATE Students SET data = '" << data << "' WHERE user_id = '" << userid << "';";
    db.execute(ss.str());
}

std::string Student::get_data(unsigned int userid) {
    ObjStore db;
    std::stringstream ss;
    ss << "SELECT user_id, data FROM Students WHERE user_id = '" << userid << "';";
    result_set_ref result = db.select(ss.str());
    std::string data = result->get_string(1);
}