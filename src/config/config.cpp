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

#include "config.hpp"
#include "../utils/date.hpp"

#include <filesystem>
#include <algorithm>
#include <iostream>

std::shared_ptr<Config> Config::instance(bool clear)
{
    static std::shared_ptr<Config> config{nullptr};

    if (config == nullptr || clear)
    {
        config = std::make_shared<Config>();
    }

    return config;
}

std::filesystem::path Config::get_real_destination_directory() const
{
    return backup.destination / backup.backup_dir_name;
}

std::filesystem::path Config::get_destination_directory() const
{
    return get_real_destination_directory() / date::get_now();
}

bool Config::is_backup_exists() const
{
    return std::filesystem::exists(get_destination_directory());
}

std::filesystem::path Config::extract_from_root_path(const std::filesystem::path &path) const
{
    const auto path_str = path.string();
    std::size_t root_size = root_path.string().size();

    if (path_str.size() < root_size)
    {
        return path;
    }

    std::string p = path_str.substr(root_size);

    return std::filesystem::path{p};
}

std::filesystem::path Config::get_destination_directory(const std::filesystem::path &path) const
{
    const auto parent_path = path.parent_path();
    const auto parent = extract_from_root_path(parent_path);

    return get_destination_directory() / parent / path.filename();
}

void Config::set_backup_configuration(BackupCommandLineOptions &&backup_options)
{
    backup = std::move(backup_options);
    action = CommandLineType::Backup;
}


template<typename T>
void merge_if_exists(bool cond, T &source, T &dest)
{
    if (cond)
    {
        dest = std::move(source);
    }
}

template<typename T>
void merge_if_exists(T &source, T &dest)
{
    merge_if_exists(source != dest, source, dest);
}

void Config::merge(const std::shared_ptr<Config> &src)
{
    merge_if_exists(src->action != CommandLineType::Unknown,
                    src->action,
                    action);

    merge_if_exists(src->backup.dry_run, backup.dry_run);
    merge_if_exists(!src->backup.strategy.empty(),
                    src->backup.strategy,
                    backup.strategy);
    merge_if_exists(src->backup.nth > 0,
                    src->backup.nth,
                    backup.nth);
    merge_if_exists(src->backup.verbose, backup.verbose);
    merge_if_exists(!src->backup.config_file.empty(),
                    src->backup.config_file,
                    backup.config_file);
    merge_if_exists(!src->backup.destination.empty(),
                    src->backup.destination,
                    backup.destination);
    merge_if_exists(!src->backup.backup_dir_name.empty(),
                    src->backup.backup_dir_name,
                    backup.backup_dir_name);
    merge_if_exists(!src->exclusion_paths.empty(),
                    src->exclusion_paths,
                    exclusion_paths);
    merge_if_exists(!src->exclusion_patterns.empty(),
                    src->exclusion_patterns,
                    exclusion_patterns);
    merge_if_exists(!src->include_directories.empty(),
                    src->include_directories,
                    include_directories);
    merge_if_exists(!src->root_path.empty(),
                    src->root_path,
                    root_path);
}

std::string Config::to_string()
{

    std::stringstream ss;
    ss
            << "dry_run: " << backup.dry_run << "\n"
            << "strategy: " << backup.strategy << "\n"
            << "nth: " << backup.nth << "\n"
            << "verbose: " << backup.verbose << "\n"
            << "config_file: " << backup.config_file << "\n"
            << "destination: " << backup.destination << "\n"
            << "backup_dir_dname: " << backup.backup_dir_name << "\n"
            << "exclusion_paths: [ ";

    for (const auto &ep: exclusion_paths)
    {
        ss << ep << ", ";
    }

    ss
            << " ] \n"
            << "exclusion_patterns: -- \n"
            << "include_directories: [ ";

    for (const auto &id: include_directories)
    {
        ss << id << ", ";
    }
    ss << " ]\n"
       << "root_path: " << root_path << "\n";

    return ss.str();
}