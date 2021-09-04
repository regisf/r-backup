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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "config_file.hpp"
#include "values/strategy.hpp"
#include "values/destination.hpp"
#include "values/configuration.hpp"

#include <string>

enum class CommandLineType {
    Unknown,
    Help,
    Init,
    Backup,
    Restore,
    Start,
    Stop
};

struct BackupOptions {
    std::string config_file;
    std::string backup_dir_name;
};


struct Config {
    bool verbose{false};
    bool dry_run{false};

    StrategyValue strategy;
    CommandLineType action;
    BackupOptions backup;
    ConfigFile configFile;
//    DestinationValue destination;
//    ConfigurationValue configuration;

    std::filesystem::path get_destination_directory() const;

    bool backup_exists() const;

    std::filesystem::path get_() const;

    std::filesystem::path get_destination_directory(const std::filesystem::path &path) const;

    bool is_destination_dir_exists(const std::filesystem::path &source) const;

private:
    std::filesystem::path remove_root_path(const std::filesystem::path &path) const;

};

#endif
