//
// Created by gryffyn on 2/26/20.
//

#ifndef USW20_P2_LOG_H
#define USW20_P2_LOG_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <string>

using namespace boost::posix_time;

class Log {
   private:
    std::ofstream logfile;
    ptime timeUTC;
    time_facet* file_facet = new time_facet("%Y%m%d-%H-%M-%S");
    time_facet* entry_facet = new time_facet("%H:%M:%S%F");

   public:
    bool verbose;
    enum level { INFO = 0, WARN = 1, ERR = 2 };
    void write(level level, const std::string& msg);
};

#endif  // USW20_P2_LOG_H
