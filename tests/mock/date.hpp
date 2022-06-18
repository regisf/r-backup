//
// Created by regis on 18/06/2022.
//

#ifndef R_BACKUP_DATE_HPP
#define R_BACKUP_DATE_HPP

#include <chrono>
#include <ctime>
#include <cstring>

namespace mock
{
    const int Year{2022};
    const int Month{04};
    const int Day{17};

    std::time_t make_time(int year = Year, int month = Month, int day = Day)
    {
        struct tm time;
        memset(&time, 0, sizeof(struct tm));

        time.tm_year = year - 1900;
        time.tm_mon = month - 1;
        time.tm_mday = day;

        return std::mktime(&time);
    }

    std::chrono::time_point<std::chrono::system_clock> now()
    {
        std::time_t time = mock::make_time();
        return std::chrono::system_clock::from_time_t(time);
    }
}
#endif //R_BACKUP_DATE_HPP
