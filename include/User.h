//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_USER_H
#define USW20_P2_USER_H

#include <string>
#include "Key.h"

class User {
   public:
    explicit User(int id, const std::string& name, const std::string& user, std::string unhashed);

   protected:
    std::string name;
    std::string user;
    static std::string hashpw(std::string& unhashed);
    std::string pass;
    std::string plain_pass;
    std::string usertype;
    void finalize();
};

#endif  // USW20_P2_USER_H
