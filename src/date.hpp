#ifndef DATE_HPP
#define DATE_HPP

#include <string>

namespace date
{
    std::string get_now();
    time_t to_time_t(const std::string & date);
}

#endif