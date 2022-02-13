/*
 * Copyright 2021 Régis FLORET <regisfloret@protonmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "command_line.hpp"
#include "config_file.hpp"
#include "values/strategy.hpp"

#include <iostream>
#include <cstdlib>
#include <memory>
#include <filesystem>

static const size_t First = 0;


CommandLine::CommandLine(int argc, char **argv)
{

    if (argc == 1)
    {
        throw CommandLineError("Missing arguments");
    }

    for (int i = 1; i < argc; ++i)
    {
        args.emplace_back(std::string(argv[i]));
    }

    m_config = std::make_shared<Config>();
}

std::shared_ptr<Config> CommandLine::parse()
{
    if (!is_known_action(args.at(First)))
    {
        std::stringstream ss;
        ss << "The action " << args.at(First) << " is not known";
        throw CommandLineError(ss.str());
    }

    m_config->configFile.read_default_config_file();
    m_config->action = action_type_from_string(args.at(First));

    // process action options
    switch (m_config->action)
    {
    case CommandLineType::Init:
        init_configuration();
        break;

    case CommandLineType::Backup:
        backup_configuration();
        break;

    case CommandLineType::Restore:
        restore_configuration();
        break;

    case CommandLineType::Start:
        break;

    case CommandLineType::Stop:
        break;

    default:
        break;
    }

    return m_config;
}

 bool CommandLine::is_known_action(const std::string &action) const
{
    return (action == "help" || action == "--help" || action == "-h" || action == "init" || action == "backup" || action == "restore");
}

std::filesystem::path CommandLine::resolve_path(const std::string &path) const
{
    try
    {
        return std::filesystem::canonical(std::filesystem::path(path));
    }
    catch (std::filesystem::filesystem_error &err)
    {
        std::stringstream ss;
        ss << "Error : The file \"" << path << "\" doesn't exist";
        throw CommandLineError(ss.str());
    }
}

void CommandLine::backup_configuration()
{
    for (int i = 1, l = args.size(); i < l; ++i)
    {
        const std::string argument{args.at(i)};
        if (!argument.compare("--help") || !argument.compare("-h"))
        {
            std::cout << R"(
r-backup backup [OPTIONS]

The options given override the default configuration file. 
Options are:
    --config-file: set the config file that contains 
    --destination: Set the destionation
    --name: Set the destination name.
    --strategy: The default backup strategy.
        Strategies are 
            keep-all: Keep all backups. You remove old ones manually
            keep-nth: Keep only nth last backup. Implies --nth argument
            remove-previous:  Keep only one backup.
            difference: Backup only the last 
    --nth: The number of backup to keep (implies --strategy keep_nth)

)";
            std::exit(0);
        }

        if (!argument.compare("--strategy")) 
        {
            if (i + 1 == l)
            {
                throw CommandLineError("Error: --strategy option needs an argument");
            }
            
//            m_config->strategy.value = StrategyValue(args.at(++i));
            continue;
        }

        if (!argument.compare("--nth")) 
        {
            if (i + 1 == l)
            {
                throw CommandLineError("Error: --nth option needs an argument");
            }
            
//            try
//            {
//                m_config->strategy.nth = NthValue(args.at(++i));
//            }
//            catch (NthValueException &e)
//            {
//                std::stringstream ss;
//                ss << "Error: " << e.message();

//                throw CommandLineError(ss.str());
//            }

            continue;
        }

        if (!argument.compare("--dry-run"))
        {
            m_config->dry_run = true;
            m_config->verbose = true;
            continue;
        }

        if (!argument.compare("--config-file"))
        {

            if (i + 1 == l)
            {
                throw CommandLineError("Error: --config-file option needs an argument");
            }

            const std::string resolved_path{resolve_path(args.at(++i))};
            m_config->configFile.parse_file(resolved_path);
            continue;
        }

        if (!argument.compare("--destination"))
        {
            if (i + 1 == l)
            {
                throw CommandLineError("Error: --destination option needs an argument");
            }

            const std::filesystem::path destFile{resolve_path(args.at(++i))};
            m_config->configFile.set_destination(destFile);
            continue;
        }

        if (!argument.compare("--name"))
        {
            if (i + 1 == l)
            {
                throw CommandLineError("Error: --name option needs an argument");
            }

            m_config->backup_dir_name = args.at(++i);
            continue;
        }

        if (!argument.compare("--verbose"))
        {
            m_config->verbose = true;
            continue;
        }

        std::stringstream ss;
        ss << "Error: Unknown option \"" << args.at(i) << "\"";
        throw CommandLineError(ss.str());
    }

}

void CommandLine::init_configuration()
{
}

void CommandLine::restore_configuration()
{
}

CommandLineType CommandLine::action_type_from_string(const std::string &action) const
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
