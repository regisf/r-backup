#include "../../src/command_line/command_line.hpp"

#include <gtest/gtest.h>


TEST(TestCommandLine, test_command_line_constructor_with_no_argument_should_throw)
{
    int argc = 1;
    const char * argv[] = { "test" };
    
    try
    {
        CommandLine commandLine(argc, (char **)argv);
        FAIL();
    }
    catch (CommandLineError& err)
    {
        SUCCEED();
    }
}

TEST(TestCommandLine, test_command_line_ctor_with_argument)
{
    int argc = 2;
    const char * argv[] = {"test_program", "argument1" };

    CommandLine commandLine{argc, (char **)argv};

    auto args = commandLine.get_args();
    ASSERT_EQ(args.at(0).compare("argument1"), 0);
}
