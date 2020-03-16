//
// Created by gryffyn on 16/03/2020.
//

#ifndef USW20_P2_LECTURER_HPP
#define USW20_P2_LECTURER_HPP

#include "User.hpp"

class Lecturer : public User {
   public:
    explicit Lecturer(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash);
    explicit Lecturer(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash,
             std::string data);
    void save_data(unsigned int userid, std::string data);
    std::string get_data(unsigned int userid);
};

#endif  // USW20_P2_LECTURER_HPP
