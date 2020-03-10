//
// Created by gryffyn on 2/26/20.
//

#ifndef USW20_P2_OBJSTORE_HPP
#define USW20_P2_OBJSTORE_HPP

#include <mariadb++/connection.hpp>

#include <string>
#include <vector>

class ObjStore {
   public:
    void init_db();
    void insert(const std::string& str);
    void create(const std::string& str);
    void select(const std::string& str);
    void finalize();

   protected:
    mariadb::account_ref acc;
    mariadb::connection_ref conn;
    void create_connection();
};

#endif  // USW20_P2_OBJSTORE_HPP
