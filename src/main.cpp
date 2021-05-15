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
#include "command_line.hpp"
#include "action/initializer.hpp"
#include "action/backup.hpp"
#include "action/path_explorer.hpp"

#include <iostream>
#include <memory>
#include <cstdlib>

enum
{
    SuccessStatusCode = EXIT_SUCCESS,
    DefaultStatusCode = EXIT_FAILURE,
    BackupActionStatus
};

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
static int do_backup(std::shared_ptr<Config_t> config)
{
    int ret_val = 0;

    std::unique_ptr<FileCopy> filecopy = std::make_unique<FileCopy>(config);
    action::Backup bk_act(config, std::move(filecopy));

    if (bk_act.can_backup())
    {
        std::vector<std::filesystem::path> pathes;

        if (!config->backup_exists())
        {
            std::cout << "Exploring directory " << config->configFile.get_root_path() << "\n";
            PathExplorer explorer(config);
            pathes = explorer.explore();
            std::cout << "Got " << pathes.size() << " files found\n";
        }
        else
        {
            std::cout << "There's nothing to backup because it is already backuped.\n";
        }

        bk_act.backup(pathes);
    }
    else
    {
        std::cerr << "Error: Unable to backup because: " << bk_act.error_message() << std::endl;
        ret_val = BackupActionStatus;
    }

    return ret_val;
}

static int process_action(std::shared_ptr<Config_t> config)
{
    int ret_val = SuccessStatusCode;

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
    }

    return ret_val;
}

int main(int argc, char **argv)
{

    int ret_val = DefaultStatusCode;

    try
    {
        CommandLine cmdLine(argc, argv);
        std::shared_ptr<Config_t> config = cmdLine.parse();
        ret_val = process_action(config);
    }
    catch (CommandLineError &err)
    {
        std::cerr << err.message << std::endl;
        display_usage();
    }

    return ret_val;
}