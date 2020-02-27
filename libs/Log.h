//
// Created by gryffyn on 2/26/20.
//

#ifndef USW20_P2_LOG_H
#define USW20_P2_LOG_H

#include <string>

class Log {
   public:
    Log();

   private:
    void write(const std::string &msg);
};

#endif  // USW20_P2_LOG_H
