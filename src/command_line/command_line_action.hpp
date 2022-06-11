#ifndef COMMANDLINEACTION_H
#define COMMANDLINEACTION_H

#include "../config/config.hpp"

#include <string>

class CommandLineAction
{
public:
    /**
     * @brief Convert a string to a enum.
     * @param action the action to parse'
     * @return The command line type. Unknown if not recognized
     */
    static CommandLineType from_string(const std::string & action);
};

#endif // COMMANDLINEACTION_H
