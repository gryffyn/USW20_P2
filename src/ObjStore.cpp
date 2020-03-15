//
// Created by gryffyn on 2/26/20.
//

#include "ObjStore.hpp"

#include <mariadb++/connection.hpp>
#include <string>

using namespace mariadb;

bool ObjStore::init_db() {
    bool status = false;
    create_connection();
    // creates Users table
    conn->execute("CREATE TABLE IF NOT EXISTS Users ("
                  "user_id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                  "name TEXT NOT NULL,"
                  "user TEXT NOT NULL UNIQUE,"
                  "pwhash TEXT NOT NULL);");
    // creates Student table
    conn->execute("CREATE TABLE IF NOT EXISTS Students ("
                  "user_id SMALLINT UNSIGNED NOT NULL PRIMARY KEY,"
                  "data MEDIUMTEXT,"
                  "last_notif TIMESTAMP,"
                  "CONSTRAINT fk_s_user_id FOREIGN KEY (user_id) REFERENCES Users (user_id)"
                  "ON DELETE CASCADE);");
    // creates Admin table
    conn->execute("CREATE TABLE IF NOT EXISTS Lecturers ("
                  "user_id SMALLINT UNSIGNED NOT NULL PRIMARY KEY,"
                  "data MEDIUMTEXT,"
                  "CONSTRAINT fk_l_user_id FOREIGN KEY (user_id) REFERENCES Users (user_id)"
                  "ON DELETE CASCADE);");
    // creates Lectuer table
    conn->execute("CREATE TABLE IF NOT EXISTS Admins ("
                  "user_id SMALLINT UNSIGNED NOT NULL PRIMARY KEY,"
                  "CONSTRAINT fk_a_user_id FOREIGN KEY (user_id) REFERENCES Users (user_id)"
                  "ON DELETE CASCADE);");
    // creates
    return status;
}

void ObjStore::create_connection() {
    acc = mariadb::account::create("localhost", "uswp20", "USWP20WRITEDB", "uswp20v2", 3306, "");
    acc->set_auto_commit(true);
    // account->set_connect_option(, 10);
    conn = connection::create(acc);
}

void ObjStore::insert(const std::string& str) {
    conn->insert(str);
}

u64 ObjStore::execute(const std::string& str) {
    unsigned long long affected = conn->execute(str);
    return affected;
}

result_set_ref ObjStore::select(const std::string& str) {
    result_set_ref result = conn->query(str);
    return result;
}

void ObjStore::finalize() { conn->disconnect(); }

