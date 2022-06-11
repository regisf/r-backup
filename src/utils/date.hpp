#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <chrono>

namespace date
{
    using now_callback = std::chrono::time_point<std::chrono::system_clock>(*)(void);

    /**
     * @brief Get the current day date as a string
     * @param now_cb The callback as injection that increase testability
     * @return The string into format YEAR (4 digits) - MONTH (2 digits) -  DATE (2 digits)
     */
    std::string get_now(now_callback now_cb = &std::chrono::system_clock::now);

    /**
     * @brief convert a string to time_t structure
     * @param date The string to convert
     * @return the time_t structure or -1 on errors
     */
    time_t to_time_t(const std::string & date);
}

#endif
