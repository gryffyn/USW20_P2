//
// Created by gryffyn on 16/03/2020.
//

#include "Lecturer.hpp"

#include <mariadb++/connection.hpp>

#include "ObjStore.hpp"

using namespace mariadb;

Lecturer::Lecturer(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute("INSERT INTO Lecturers(user_id) VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1))");
}

Lecturer::Lecturer(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash,
                 std::string data)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute(
        "INSERT INTO Lecturers(user_id, data) "
        "VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1), '" + data + "');");
}

void Lecturer::save_data(unsigned int userid, std::string data) {
    std::stringstream ss;
    ss << "UPDATE Lecturers SET data = '" << data << "' WHERE user_id = '" << userid << "';";
    db.execute(ss.str());
}

std::string Lecturer::get_data(unsigned int userid) {
    std::stringstream ss;
    ss << "SELECT user_id, data FROM Lecturers WHERE user_id = '" << userid << "';";
    result_set_ref result = db.select(ss.str());
    return result->get_string(1);
}
