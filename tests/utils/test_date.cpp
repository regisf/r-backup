#include "../../src/utils/date.hpp"
#include "../mock/date.hpp"

#include <gtest/gtest.h>


TEST(TestDate, test_date_now)
{
    // Arrange
    std::string expected_result{"2022-04-17"};

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

    std::set<std::string, std::less<>> times = {
            "23-11-12",     // Not the right size for years
            "2022.12-12",   // not the good separator
            "2022-1-12",    // Not the right size for month
            "2022-12-1"     // Not the right size for day
    };

    for (const auto &time: times)
    {
        // Act
        time_t result = date::to_time_t(time);

        // Assert
        ASSERT_EQ(expected_time, result);
    }
}
