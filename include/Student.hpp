//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_STUDENT_HPP
#define USW20_P2_STUDENT_HPP

#include "User.hpp"

class Student : public User {
   public:
    explicit Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash);
    explicit Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash, std::string password, std::string data);
};

#endif  // USW20_P2_STUDENT_HPP
