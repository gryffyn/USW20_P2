//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_USER_HPP
#define USW20_P2_USER_HPP

#include <mariadb++/connection.hpp>
#include <string>

#include "Key.hpp"
#include "ObjStore.hpp"

class User {
   public:
    explicit User(ObjStore& db, const std::string& name, const std::string& user, std::string unhashed);
    mariadb::u64 creation_result{};

   protected:
    static std::string hashpw(std::string& unhashed);
    ObjStore db;
};

#endif  // USW20_P2_USER_HPP
