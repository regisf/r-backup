#include <gtest/gtest.h>

#include "../../src/command_line/backup_command_line.hpp"
#include "../../src/command_line/command_line.hpp"
#include "../mock/exit.hpp"


TEST(TestBackupCommandLine, test_backup_command_line_with_args_should_throw_on_unknown_argument)
{
    auto cmdLine = BackupCommandLine({"backup"});

    cmdLine.parse();
    SUCCEED();
}

TEST(TestBackupCommandLine, test_long_help_argument_should_exit)
{
    // Arrange
    auto cmdLine = BackupCommandLine({"backup", "--help"});
    mock::exit_setup();

    // act
    (void) cmdLine.parse(mock::exit);

    ASSERT_TRUE(mock::is_exit_called());
    mock::exit_tear_down();

    cmdLine = BackupCommandLine({"backup", "-h"});
    cmdLine.parse(mock::exit);

    ASSERT_TRUE(mock::is_exit_called());
    mock::exit_tear_down();

    cmdLine = BackupCommandLine({"backup", "help"});
    cmdLine.parse(mock::exit);
    ASSERT_TRUE(mock::is_exit_called());
    mock::exit_tear_down();
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

    auto options = std::move(cmdLine.parse());

    ASSERT_EQ(options.strategy, std::string{"keep_all"});
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

    auto options = std::move(cmdLine.parse());

    std::stringstream ss;
    ss << expected_nth;
    ss >> expected_nth_int;

    ASSERT_EQ(options.nth, expected_nth_int);
}

TEST(TestBackupCommandLine, test_dry_run_with_value)
{
    auto cmdLine = BackupCommandLine({"backup", "--dry-run"});

    auto options = std::move(cmdLine.parse());

    ASSERT_TRUE(options.dry_run);
}

TEST(TestBackupCommandLine, test_dry_run_default_value_is_false)
{
    auto cmdLine = BackupCommandLine({"backup"});

    auto options = std::move(cmdLine.parse());

    ASSERT_FALSE(options.dry_run);
}

TEST(TestBackupCommandLine, test_config_file_with_value_should_load_file)
{
    auto expected_file_name = "aConfigFile";
    auto cmdLine = BackupCommandLine({"backup", "--config-file", expected_file_name});

    auto options = std::move(cmdLine.parse());

    ASSERT_EQ(expected_file_name, options.config_file);
}

TEST(TestBackupCommandLine, test_config_file_without_value_should_fail)
{
    auto cmdLine = BackupCommandLine({"backup", "--config-file"});

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

TEST(TestBackupCommandLine, test_destination_with_value)
{
    auto expected_destination = "a-destination-directory";
    auto cmdLine = BackupCommandLine({"backup", "--destination", expected_destination});

    auto options = std::move(cmdLine.parse());

    ASSERT_EQ(expected_destination, options.destination);
}

TEST(TestBackupCommandLine, test_backup_dir_name_with_value)
{
    auto expected_name = "backupname";
    auto cmdLine = BackupCommandLine({"backup", "--backup-directory-name", expected_name});
    auto options = std::move(cmdLine.parse());
    ASSERT_EQ(expected_name, options.backup_dir_name);
}

TEST(TestBackupCommandLine, test_backup_dir_name_without_value_should_fail)
{
    auto cmdLine = BackupCommandLine({"backup", "--backup-directory-name"});

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

TEST(TestBackupCommandLine, test_verbose_argument)
{
    auto cmdLine = BackupCommandLine({"backup", "--verbose"});

    auto options = std::move(cmdLine.parse());

    ASSERT_TRUE(options.verbose);
}

TEST(TestBackupCommandLine, test_verbose_is_false_by_default)
{
    auto cmdLine = BackupCommandLine({"backup"});

    auto options = std::move(cmdLine.parse());

    ASSERT_FALSE(options.verbose);
}
