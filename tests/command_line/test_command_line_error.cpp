#include "../../src/command_line/command_line.hpp"
#include <gtest/gtest.h>


TEST(TestCommandLineError, test_command_line_error_without_argument)
{
    CommandLineError err;
    ASSERT_STREQ(err.what(), "");
}

TEST(TestCommandLineError, test_command_line_error_with_argument)
{
    const char * expected = "expected string";
    CommandLineError err{expected};

    ASSERT_STREQ(err.what(), expected);
}
