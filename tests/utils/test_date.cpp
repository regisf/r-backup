#include "../../src/utils/date.hpp"

#include <gtest/gtest.h>
#include <chrono>
#include <ctime>


namespace mock {
    const int Year{2022};
    const int Month{04};
    const int Day{17};

    std::time_t make_time(int year = Year, int month = Month, int day = Day) {
        struct tm time;
        memset(&time, 0, sizeof(struct tm));

        time.tm_year = year - 1900;
        time.tm_mon = month -1;
        time.tm_mday = day;

        return std::mktime(&time);
    }

    std::chrono::time_point<std::chrono::system_clock> now() {
        std::time_t time = mock::make_time();
        return std::chrono::system_clock::from_time_t(time);
    }
}

TEST(TestDate, test_date_now)
{
    // Arrange
    std::string  expected_result{"2022-04-17"};

    // Act
    std::string result = date::get_now(mock::now);

    // Assert
    ASSERT_EQ(expected_result, result);
}

TEST(TestDate, test_date_to_time_t)
{
    // Arrange
    time_t expected_time = mock::make_time();
    std::string Today{"2022-04-17"};

    // Act
    time_t result = date::to_time_t(Today);

    // Assert
    ASSERT_EQ(expected_time, result);
}

TEST(TestDate, test_to_time_t_with_inconsitant_date)
{
    // Arrange
    time_t expected_time = -1;

    std::set<std::string> times = {
        "23-11-12",     // Not the right size for years
        "2022.12-12",   // not the good separator
        "2022-1-12",    // Not the right size for month
        "2022-12-1"     // Not the right size for day
    };

    for (const auto &time : times) {
        // Act
        time_t result = date::to_time_t(time);

        // Assert
        ASSERT_EQ(expected_time, result);
    }
}
