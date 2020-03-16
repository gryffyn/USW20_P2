//
// Created by gryffyn on 16/03/2020.
//

#ifndef USW20_P2_ADMIN_HPP
#define USW20_P2_ADMIN_HPP

#include "User.hpp"

class Admin : public User{
   public:
    explicit Admin(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash);
};

#endif  // USW20_P2_ADMIN_HPP
