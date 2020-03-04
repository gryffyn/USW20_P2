//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_USER_H
#define USW20_P2_USER_H

#include <string>
#include "Key.h"

class User {
   public:
    char ID[8];
    struct user_details_s {
        std::string user_id;
        std::string user_name;
        std::string user_user;
        std::string user_age;
    };
   protected:
    explicit User(char id[8], const std::string& name, std::string user, std::string unhashed);
    std::string name;
    std::string user;
    static std::string hashpw(std::string& unhashed);
    std::string pass;
    std::string plain_pass;
    ~User();
    user_details_s user_details;
    void finalize();
    void get_user();
};

#endif  // USW20_P2_USER_H
