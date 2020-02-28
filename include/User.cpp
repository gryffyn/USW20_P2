//
// Created by gryffyn on 18/02/2020.
//

#include "User.h"
#include "Key.h"

void User::hashpw(std::string unhashed) {
    Key key(unhashed);
    pass = key.get_key();
}
