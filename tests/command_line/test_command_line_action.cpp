#include "../../src/config.hpp"
#include "../../src/command_line/command_line_action.hpp"

#include <gtest/gtest.h>

TEST(TestCommandLineAction, test_command_line_action_with_correct_action)
{
    CommandLineType type;

    type = CommandLineAction::from_string("help");
    ASSERT_EQ(type, CommandLineType::Help);

    type = CommandLineAction::from_string("--help");
    ASSERT_EQ(type, CommandLineType::Help);

    type = CommandLineAction::from_string("-h");
    ASSERT_EQ(type, CommandLineType::Help);

    type = CommandLineAction::from_string("init");
    ASSERT_EQ(type, CommandLineType::Init);

    type = CommandLineAction::from_string("backup");
    ASSERT_EQ(type, CommandLineType::Backup);

    type = CommandLineAction::from_string("restore");
    ASSERT_EQ(type, CommandLineType::Restore);

    type = CommandLineAction::from_string("start");
    ASSERT_EQ(type, CommandLineType::Start);

    type = CommandLineAction::from_string("stop");
    ASSERT_EQ(type, CommandLineType::Stop);

    type = CommandLineAction::from_string("truc-bidule-machin");
    ASSERT_EQ(type, CommandLineType::Unknown);
}
