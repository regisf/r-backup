#include "date.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <regex>

namespace date
{
    std::string get_now()
    {
        const auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;

        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
        return ss.str();
    }

    time_t to_time_t(const std::string & date)
    {
        std::regex regex{"(\\d{4})-(\\d{2})-(\\d{2})"};
        std::smatch match;
        if (!std::regex_match(date, match, regex)) 
        {
            return -1;
        }


        std::tm tm;
        tm.tm_year = std::stoi(match[1]);
        tm.tm_mon = std::stoi(match[2]);
        tm.tm_mday = std::stoi(match[3]);

        return std::mktime(&tm);
    }
}