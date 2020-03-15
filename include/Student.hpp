//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_STUDENT_HPP
#define USW20_P2_STUDENT_HPP
#include "User.hpp"
#include "ObjStore.hpp"

class Student : public User {
   public:
    explicit Student(ObjStore& db, const std::string& name, const std::string& user, std::string unhashed);
    Student(ObjStore& u_db, const std::string& name, const std::string& user, std::string unhashed, std::string data);
    void save_data(unsigned int userid, std::string data);
    std::string get_data(unsigned int userid);

   protected:
    ObjStore db;
};

#endif  // USW20_P2_STUDENT_HPP
