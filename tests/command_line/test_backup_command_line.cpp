#include <gtest/gtest.h>

#include "../../src/command_line/backup_command_line.hpp"
#include "../../src/command_line.hpp"


TEST(TestBackupCommandLine, test_backup_command_line_with_args_should_throw_on_unknown_argument)
{
    auto cmdLine = BackupCommandLine({"backup"});

    cmdLine.parse();
    SUCCEED();
}

namespace mock
{
    bool exit_called = false;
    void exit(int status)
    {
        exit_called=true;
    }
}

TEST(TestBackupCommandLine, test_long_help_argument_should_exit)
{
    auto cmdLine = BackupCommandLine({"backup", "--help"});

    cmdLine.parse(mock::exit);

    ASSERT_TRUE(mock::exit_called);
    mock::exit_called = false;

    cmdLine = BackupCommandLine({"backup", "-h"});
    cmdLine.parse(mock::exit);

    ASSERT_TRUE(mock::exit_called);
    mock::exit_called = false;
}

TEST(TestBackupCommandLine, test_strategy_without_value_only_should_fail)
{
    auto cmdLine = BackupCommandLine({"backup", "--strategy"});

    try
    {
        cmdLine.parse();
        FAIL();
    }
    catch (CommandLineError &err)
    {
        SUCCEED();
    }
}

TEST(TestBackupCommandLine, test_strategy_with_value)
{
    auto cmdLine = BackupCommandLine({"backup", "--strategy", "keep_all"});

    cmdLine.parse();

    ASSERT_EQ(cmdLine.get_strategy(), std::string{"keep_all"});
}

TEST(TestBackupCommandLine, test_nth_without_value_should_fail)
{
    auto cmdLine = BackupCommandLine({"backup", "--nth"});

    try
    {
        cmdLine.parse();
        FAIL();
    }
    catch (CommandLineError &err)
    {
        SUCCEED();
    }
}

TEST(TestBackupCommandLine, test_nth_with_value)
{
    std::string expected_nth{"10"};
    int expected_nth_int;
    auto cmdLine = BackupCommandLine({"backup", "--nth", expected_nth});

    cmdLine.parse();

    std::stringstream ss;
    ss << expected_nth;
    ss >> expected_nth_int;

    ASSERT_EQ(cmdLine.get_nth(), expected_nth_int);
}
