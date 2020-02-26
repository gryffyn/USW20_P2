//
// Created by gryffyn on 2/26/20.
//

#include "ObjStore.h"
#include <sqlite3.h>
#include <string>
#include <iostream>

class db_exception : public std::exception {
    [[nodiscard]] const char * what () const noexcept override{
        return "Failed to open database.";
    }
};

void ObjStore::open_db(const std::string& dbname){
    if (sqlite3_open(dbname.c_str(), &db) != 0) {
        throw db_exception();
    }
}

ObjStore::ObjStore(const std::string& dbname){
    ObjStore::open_db(dbname);
}

void ObjStore::close_db(){
    sqlite3_close(db);
}

