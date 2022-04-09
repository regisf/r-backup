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
#include "command_line_action.hpp"
#include "../config_file_parser.hpp"
#include "../values/strategy.hpp"

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

    //m_config = ConfigFileParser::read_default_config_file()->to_config();
}

std::shared_ptr<Config> CommandLine::parse()
{    
    auto action = CommandLineAction::from_string(args.at(First));

    switch (action)
    {
    case CommandLineType::Init:
        init_configuration();
        break;

    case CommandLineType::Backup:
        //BackupCommandLine(get_args());
        backup_configuration();
        break;

    case CommandLineType::Restore:
        restore_configuration();
        break;

    case CommandLineType::Start:
    case CommandLineType::Stop:
    default:
        {
            std::stringstream ss;
            ss << "The action " << args.at(First) << " is not known";
            throw CommandLineError(ss.str());
        }
    }

    return m_config;
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


        if (!argument.compare("--dry-run"))
        {
            m_config->dry_run = true;
            continue;
        }

        if (!argument.compare("--config-file"))
        {

            if (i + 1 == l)
            {
                throw CommandLineError("Error: --config-file option needs an argument");
            }

            auto resolved_path{resolve_path(args.at(++i))};
            auto config_red = ConfigFileParser::read_default_config_file(resolved_path)->to_config();
            m_config->merge(config_red);

            continue;
        }

        if (!argument.compare("--destination"))
        {
            if (i + 1 == l)
            {
                throw CommandLineError("Error: --destination option needs an argument");
            }

            const std::filesystem::path destFile{resolve_path(args.at(++i))};
            m_config->destination = destFile;
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

std::vector<std::string> CommandLine::get_args() noexcept
{
    return args;
}
