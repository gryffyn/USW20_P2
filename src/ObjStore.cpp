//
// Created by gryffyn on 2/26/20.
//

#include "ObjStore.hpp"

#include <mariadb++/connection.hpp>
#include <string>

using namespace mariadb;

void ObjStore::init_db() {
    create_connection();
    // creates Users table
    conn->execute("CREATE TABLE IF NOT EXISTS Users ("
                  "user_id INT NOT NULL PRIMARY KEY,"
                  "name TEXT NOT NULL,"
                  "user TEXT NOT NULL,"
                  "pwhash TEXT NOT NULL);");
    // creates Student table
    conn->execute("CREATE TABLE IF NOT EXISTS Students ("
                  "user_id INT NOT NULL PRIMARY KEY,"
                  "data MEDIUMTEXT,"
                  "last_notif datetime,"
                  "CONSTRAINT fk_s_user_id FOREIGN KEY (user_id) REFERENCES Users (user_id));");
    // creates Admin table
    conn->execute("CREATE TABLE IF NOT EXISTS Lecturers ("
                  "user_id INT NOT NULL PRIMARY KEY,"
                  "data MEDIUMTEXT,"
                  "CONSTRAINT fk_l_user_id FOREIGN KEY (user_id) REFERENCES Users (user_id));");
    // creates Lectuer table
    conn->execute("CREATE TABLE IF NOT EXISTS Admins ("
                  "user_id INT NOT NULL PRIMARY KEY,"
                  "CONSTRAINT fk_a_user_id FOREIGN KEY (user_id) REFERENCES Users (user_id));");
    // creates
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

