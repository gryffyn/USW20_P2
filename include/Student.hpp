//
// Created by gryffyn on 18/02/2020.
//

#ifndef USW20_P2_STUDENT_HPP
#define USW20_P2_STUDENT_HPP
#include "User.hpp"

class Student : public User {
   public:
    explicit Student(ObjStore& db, int id, const std::string& name, const std::string& user, std::string unhashed);
    explicit Student(ObjStore& db, const std::string& name, const std::string& user, std::string unhashed);

   protected:
    void save_data(unsigned int userid, std::string data);
    std::string get_data(unsigned int userid);
};

#endif  // USW20_P2_STUDENT_HPP
