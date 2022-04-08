#include "command_line_action.h"

CommandLineType CommandLineAction::from_string(const std::string & action)
{
    CommandLineType type = CommandLineType::Unknown;

    if (action == "help" || action == "--help" || action == "-h")
    {
        type = CommandLineType::Help;
    }
    else if (action == "init")
    {
        type = CommandLineType::Init;
    }
    else if (action == "backup")
    {
        type = CommandLineType::Backup;
    }
    else if (action == "restore")
    {
        type = CommandLineType::Restore;
    }
    else if (action == "start")
    {
        type = CommandLineType::Start;
    }
    else if (action == "stop")
    {
        type = CommandLineType::Stop;
    }

    return type;
}
