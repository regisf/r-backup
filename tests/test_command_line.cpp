#include "../src/command_line.hpp"

#include <gtest/gtest.h>

TEST(TestCommandLine, test_command_line_constructor_with_no_argument_should_exit)
{
    
    int argc = 1;
    char * argv[argc] = {"test"};
    
    CommandLine commandLine(argc, argv);
    
}
