//
// Created by gryffyn on 2/26/20.
//

#include "ObjStore.h"

#include <sqlite3.h>

#include <filesystem>
#include <string>
#include <vector>

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

class db_exception : public std::exception {
    [[nodiscard]] const char* what() const noexcept override { return "Failed to open database."; }
};

void ObjStore::open_db(const std::string& dbname) {
    if (sqlite3_open(dbname.c_str(), &db) != 0) { throw db_exception(); }
}

void ObjStore::init_db() {
    std::vector<std::string> types;
    types.emplace_back("Users");
    types.emplace_back("Students");
    types.emplace_back("Lecturers");
    types.emplace_back("Admins");
    for (std::string str : types) {
        std::string sql = ("CREATE TABLE " + str +
            "(ID INT PRIMARY KEY NOT NULL,"
            "NAME TEXT NOT NULL,"
            "USER TEXT NOT NULL,"
            "PWHASH TEXT NOT NULL);");
        char* zErrMsg;
        int sqlcode = sqlite3_exec(db, sql.c_str(), callback, nullptr, &zErrMsg);
    }
}

ObjStore::ObjStore(const std::string& dbname) {
    ObjStore::open_db(dbname);
    if (!std::filesystem::exists("dbname")) { ObjStore::init_db(); }
}

void ObjStore::close_db() { sqlite3_close(db); }
