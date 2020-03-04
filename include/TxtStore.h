//
// Created by gryffyn on 04/03/2020.
//

#ifndef USW20_P2_TXTSTORE_H
#define USW20_P2_TXTSTORE_H

#include <fstream>

#include "User.h"
class TxtStore {
   public:
    TxtStore();
    void save_user(User user);

   private:
    std::ofstream user_file;
    void initialize(User user);
};

#endif  // USW20_P2_TXTSTORE_H
