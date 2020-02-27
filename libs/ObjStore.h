//
// Created by gryffyn on 2/26/20.
//

#ifndef USW20_P2_OBJSTORE_H
#define USW20_P2_OBJSTORE_H

#include <sqlite3.h>

#include <string>

class ObjStore {
   public:
    explicit ObjStore(const std::string& dbname);
    void close_db();

   private:
    void open_db(const std::string& dbname);
    sqlite3* db;
    void init_db();
};

#endif  // USW20_P2_OBJSTORE_H
