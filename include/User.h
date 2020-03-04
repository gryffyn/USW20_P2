//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_USER_H
#define USW20_P2_USER_H

#include <string>
#include "Key.h"

class User {
   public:
   protected:
    explicit User(char id[8], const std::string& name, std::string user, std::string unhashed);
    std::string name;
    std::string user;
    char ID[8]{};
    static std::string hashpw(std::string& unhashed);
    std::string pass;
    std::string plain_pass;
    ~User();

   private:
    void finalize();
    struct User_details {
        char id[8];
        std::string user_name;
        std::string user_user;
        std::string user_pwhash;
    };
};

#endif  // USW20_P2_USER_H
