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
#include "command_line.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>

#include <lyra/lyra.hpp>
#include <spdlog/spdlog.h>

#define RBackupVersion "0.1.0"

enum class StatusCode {
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
static StatusCode do_backup(const std::shared_ptr<Config>& config)
{
    StatusCode ret_val{StatusCode::DefaultStatusCode};

    FileCopy filecopy{config};

    if (action::Backup bk_act(config, filecopy); bk_act.can_backup()) {
        std::vector<std::filesystem::path> pathes;

        if (!config->backup_exists()) {
            spdlog::info("Exploring directory {}",config->configFile.get_root_path());
            PathExplorer explorer(config);
            pathes = explorer.explore();
            spdlog::info("Got {} files found", pathes.size());
        } else {
            std::cout
                    << "There's nothing to backup because it is already backuped.\n";
        }

        bk_act.backup(pathes);
    } else {
        std::cerr << "Error: Unable to backup because: " << bk_act.error_message()
                  << std::endl;
        ret_val = StatusCode::BackupActionStatus;
    }

    return ret_val;
}

static StatusCode process_action(const std::shared_ptr<Config>& config)
{
    auto ret_val = StatusCode::SuccessStatusCode;

    switch (config->action) {
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

static std::shared_ptr<Config> parse_commandline(int argc, char** argv)
{
    bool show_help { false };
    std::string strategy;
    std::string config_file;
    std::string destination;
    int nth{0};

    auto config = std::make_shared<Config>();
    
    auto cli = lyra::cli() | lyra::help(show_help);

    auto version_group = lyra::command("version", [](const lyra::group& g) { 
        std::cout << "R-Backup - Version " << RBackupVersion << std::endl;
        std::exit(EXIT_SUCCESS);
    }).help("Display the current version");

    auto backup_group = lyra::command("backup", [](const lyra::group& g) {})
            .help("Display backup help")
            .add_argument(lyra::opt(config->verbose)["--verbose"]("Set verbosity to on").optional())
            .add_argument(lyra::opt(config->dry_run)["--dry-run"]("Do nothing, just simulate").optional())
            .add_argument(lyra::opt(strategy, "strategy")["--strategy"]("Set the backup strategy").optional())
            .add_argument(lyra::opt(nth, "nth")["--nth"]("How many").optional())
            .add_argument(lyra::opt(destination, "destination")["--destination"]("Set the destination directory").optional())
            .add_argument(lyra::opt(config_file, "configuration_file")["--config-file"]("set the config file that contains ").optional());

    cli.add_argument(version_group);
    cli.add_argument(backup_group);
    cli.add_argument(version_group);

    if (auto result = cli.parse({ argc, argv }); !result) {
        std::cerr << "Error in command line: " << result.errorMessage()
                  << std::endl;
        std::exit(1);
    }

    if (show_help) {
        std::cout << cli << "\n";
        std::exit(EXIT_SUCCESS);
    }

    return config;
}

int main(int argc, char** argv)
{
    auto config = parse_commandline(argc, argv);

    // std::cout << "Verbose = " << config->verbose << "\n"
    //            << "Dry run = " << config->dry_run << "\n";
    StatusCode ret_val = StatusCode::DefaultStatusCode;

    try
    {
        CommandLine cmdLine(argc, argv);
        auto config = cmdLine.parse();
        ret_val = process_action(config);
    }
    catch (const CommandLineError &err)
    {
        std::cerr << err.message << std::endl;
        display_usage();
    }

    return int(ret_val);
}
