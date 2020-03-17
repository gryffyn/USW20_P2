//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_STUDENT_HPP
#define USW20_P2_STUDENT_HPP

#include "User.hpp"

namespace DataTools {
void save_data(ObjStore& db, unsigned int userid, std::string data, std::string password);
std::string get_data(ObjStore& db, unsigned int userid, std::string password);
std::string get_data_xor(ObjStore& db, unsigned int userid, std::string passwd);
void save_data_xor(ObjStore& db, unsigned int userid, std::string data, std::string passwd);
std::string return_data_xor(ObjStore& db, unsigned int userid, std::string data, std::string passwd);
}

class Student : public User {
   public:
    explicit Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash);
    explicit Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash, std::string password, std::string data);
};

#endif  // USW20_P2_STUDENT_HPP
