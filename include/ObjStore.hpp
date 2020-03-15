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
    bool init_db();
    void insert(const std::string& str);
    mariadb::u64 execute(const std::string& str);
    mariadb::result_set_ref select(const std::string& str);
    void finalize();

   protected:
    mariadb::account_ref acc{};
    mariadb::connection_ref conn{};
    void create_connection();
};

#endif  // USW20_P2_OBJSTORE_HPP
