//
// Created by gryffyn on 10/03/2020.
//

#ifndef USW20_P2_USERMENU_HPP
#define USW20_P2_USERMENU_HPP

#include <string>
#include "ObjStore.hpp"

namespace Login {
int login_menu(ObjStore& db);
}

namespace UserMenu {
void show_menu(ObjStore& db, const int& user_id);
}

#endif  // USW20_P2_USERMENU_HPP
