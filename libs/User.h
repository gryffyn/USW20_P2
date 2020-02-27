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
    std::string name;
    std::string user;
    char ID[8]{};
    void hashpw(const std::string& unhashed);
    std::string pass;
    std::string plain_pass;
   private:
};

#endif  // USW20_P2_USER_H
