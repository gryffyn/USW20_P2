//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_USER_HPP
#define USW20_P2_USER_HPP

#include <string>

#include "Key.hpp"

class User {
   public:
    explicit User(int id, const std::string& name, const std::string& user, std::string unhashed);

   protected:
    static std::string hashpw(std::string& unhashed);
};

#endif  // USW20_P2_USER_HPP
