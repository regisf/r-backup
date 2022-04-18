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

/**
 * Usage:  r-backup [COMMANDS] [OPTIONS]
 *  commands are
 *      help
 *      init
 *      config
 *      backup
 *      restore
 */
#include "action/backup.hpp"
#include "action/initializer.hpp"
#include "action/path_explorer.hpp"
#include "command_line/command_line.hpp"
#include "config_file_parser.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>

// #include <lyra/lyra.hpp>
// #include <spdlog/spdlog.h>

#define RBackupVersion "0.1.0"

enum class StatusCode
{
    SuccessStatusCode = EXIT_SUCCESS,
    DefaultStatusCode = EXIT_SUCCESS,
    BackupActionStatus = 2
};

static const std::string_view CurrentVersion{"1.0.0"};

static void display_usage()
{
    std::cout << R"(r-backup - Event driven backup system.

Usage:
    r-backup ACTION [OPTIONS]

Actions are:
    - version: Display the version number
    - help: Display this message.
    - init: Initialize first time launch
    - backup: Backup regarding the configuration file
    - restore: Restore a previous backup
    - start: Start in backgroup 

Options:
    type "action" followed by "--help"

)";
}

/**
 * Process the backup action.
 * The return code is 2
 */
static StatusCode do_backup(const std::shared_ptr<Config> &config)
{
    StatusCode ret_val{StatusCode::DefaultStatusCode};

    FileCopy filecopy{config};
    if (action::Backup bk_act(config, filecopy); bk_act.can_backup())
    {
        std::vector<std::filesystem::path> pathes;

        if (!config->is_backup_exists())
        {
            // spdlog::info("Exploring directory {}",config->root_path);
            std::cout << "Exploring directory " << config->root_path << "\n";
            PathExplorer explorer(config);
            pathes = explorer.explore();
            std::cout << "Got " << pathes.size() << " files found\n";
            // spdlog::info("Got {} files found", pathes.size());
        }
        else
        {
            std::cout << "There's nothing to backup because it is already backuped.\n";
        }

        bk_act.backup(pathes);
    }
    else
    {
        std::cerr << "Error: Unable to backup because: " << bk_act.error_message()
                  << std::endl;
        ret_val = StatusCode::BackupActionStatus;
    }

    return ret_val;
}

static StatusCode process_action(const std::shared_ptr<Config> &config)
{
    auto ret_val = StatusCode::SuccessStatusCode;

    switch (config->action)
    {
    case CommandLineType::Help:
        display_usage();
        break;

    case CommandLineType::Init:
        action::initialize();
        break;

    case CommandLineType::Backup:
        ret_val = do_backup(config);
        break;

    case CommandLineType::Restore:
        break;

    case CommandLineType::Start:
        break;

    case CommandLineType::Stop:
        break;

    case CommandLineType::Unknown:
        break;
    }

    return ret_val;
}

int main(int argc, char **argv)
{
    StatusCode ret_val = StatusCode::DefaultStatusCode;

    try
    {
        CommandLine cmdLine(argc, argv);
        auto config = cmdLine.parse();

        std::shared_ptr<ConfigFileParser> config_parser;

        if (! config->backup.config_file.empty()) {
            config_parser = ConfigFileParser::read_default_config_file(config->backup.config_file);
        } else {
            config_parser = ConfigFileParser::read_default_config_file();
        }

        std::shared_ptr<Config> file_config = config_parser->to_config();

        config->merge(file_config);

        ret_val = process_action(config);
    }
    catch (const CommandLineError &err)
    {
        std::cerr << err.message << std::endl;
        display_usage();
    }

    return int(ret_val);
}
