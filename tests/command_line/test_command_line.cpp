#include "../../src/command_line/command_line.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <string>

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

TEST(TestCommandLive, test_unknown_command_failed)
{
    // Arrange
    const char * argv[] = {"test_program","unknown_command"};
    CommandLine cmdLine{2, (char **) argv};

    // Act
    try
    {
        auto options = cmdLine.parse();
        // Assert
        FAIL();
    }
    catch (CommandLineError& err)
    {
        // Assert
        SUCCEED();
    }
}

TEST(TestCommandLine, test_parse_actions)
{
    // Arrange
    std::vector<const char *> actions = {
        "backup",
        "help", "-h", "--help",
        "init",
    };

    // Act && assert
    for(const char* action : actions)
    {
        const char * a[] = {"test_program", (char*)action};
        CommandLine cmd_line{2, (char **) a};
        (void)cmd_line.parse();
    }
}

TEST(TestCommandLine, test_parse_with_backup_values_are_merged)
{
    // Arrange
    std::filesystem::path expected_destination{"/home/fake"};
    std::filesystem::path expected_config_file{ "/home/fake/config.yml" };
    std::string expected_dir_name{"backup-dir-name"};
    std::string expected_strategy{"keep-all"};

    const char * argv[] = {
        "test_program",
        "backup",
        "--verbose",
        "--dry-run",
        "--destination", expected_destination.c_str(),
        "--backup-directory-name", expected_dir_name.c_str(),
        "--config-file", expected_config_file.c_str(),
        "--strategy", "keep-all"
    };

    CommandLine cmd_line{12, (char **)argv};

    // Act
    auto options = cmd_line.parse();

    // Assert
    ASSERT_EQ(options->action, CommandLineType::Backup);
    ASSERT_TRUE(options->backup.verbose);
    ASSERT_TRUE(options->backup.dry_run);
    ASSERT_EQ(options->backup.destination, expected_destination);
    ASSERT_EQ(options->backup.config_file, expected_config_file);
    ASSERT_EQ(options->backup.backup_dir_name, expected_dir_name);
    ASSERT_EQ(options->backup.strategy, expected_strategy);
}
