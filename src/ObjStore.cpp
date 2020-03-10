//
// Created by gryffyn on 2/26/20.
//

#include "ObjStore.h"

#include <mariadb++/connection.hpp>
#include <string>

using namespace mariadb;

void ObjStore::init_db() {
    create_connection();
    std::string sql_command = ("CREATE TABLE IF NOT EXISTS Users ("
                               "user_id INT NOT NULL PRIMARY KEY,"
                               "user_type ENUM('Student', 'Lecturer', 'Admin') NOT NULL,"
                               "name TEXT NOT NULL,"
                               "user TEXT NOT NULL,"
                               "pwhash TEXT NOT NULL,"
                               "data MEDIUMTEXT,"
                               "last_notif DATE);");
    conn->execute(sql_command);
}

void ObjStore::create_connection() {
    acc = mariadb::account::create("localhost", "uswp20", "USWP20WRITEDB", "uswp20", 3306, "");
    acc->set_auto_commit(true);
    // account->set_connect_option(, 10);
    conn = connection::create(acc);
}

void ObjStore::insert(const std::string& str) {
    conn->insert(str);
}

void ObjStore::create(const std::string& str) {
    conn->execute(str);
}

void ObjStore::select(const std::string& str) {
    conn->query(str);
}

void ObjStore::finalize() { conn->disconnect(); }

