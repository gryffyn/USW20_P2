//
// Created by gryffyn on 17/03/2020.
//

#ifndef USW20_P2_ANNOUNCEMENT_HPP
#define USW20_P2_ANNOUNCEMENT_HPP
#include <string>
#include "ObjStore.hpp"

class Announcement {
   public:
    explicit Announcement(ObjStore& u_db, const int& id, std::string i_title, std::string i_text);

   protected:
    ObjStore db;
};

#endif  // USW20_P2_ANNOUNCEMENT_HPP
