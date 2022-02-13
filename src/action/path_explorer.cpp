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

#include "path_explorer.hpp"
#include "../command_line.hpp"
#include "../date.hpp"

#include <algorithm>
#include <regex>
#include <iostream>


PathExplorer::PathExplorer(std::shared_ptr<Config> config)
    : m_config(config) {}

std::vector<std::filesystem::path> PathExplorer::explore()
{
    auto directories = m_config->configFile.get_included_directories();
    // auto last_backup_date = guess_last_backup();

    // No included directory so use backup root directory
    if (!directories.size())
    {
        directories.push_back(m_config->configFile.get_root_path());
    }

    std::for_each(directories.begin(), directories.end(), [&](const auto &path) { explore_directory(path); });

    return m_pathes;
}

const bool PathExplorer::should_be_skipped(const std::filesystem::path &p) const
{
    if (std::filesystem::is_symlink(p))
    {
        return true;
    }

    bool is_excluded = false;
    auto &patterns = m_config->configFile.get_excluded_patterns();

    for (const auto &pattern : patterns)
    {
        if (std::regex_search(p.string(), pattern))
        {
            is_excluded = true;
            break;
        }
    }

    if (!is_excluded)
    {
        auto &pathes = m_config->configFile.get_excluded_paths();
        for (const auto &path : pathes)
        {
            if (p.string().starts_with(path))
            {
                is_excluded = true;
                break;
            }
        }
    }

    return is_excluded;
}

void PathExplorer::explore_directory(const std::filesystem::path &dir_path)
{

    if (!std::filesystem::exists(dir_path))
    {
        std::cerr << dir_path << " doesn't exist. Skipping\n";
        return;
    }
    
    if (std::filesystem::is_regular_file(dir_path) and !should_be_skipped(dir_path))
    {
        m_pathes.push_back(dir_path);
        return;
    }

     for (auto &p : std::filesystem::directory_iterator(dir_path))
    {

        if (should_be_skipped(p))
        {
            continue;
        }

        if (std::filesystem::is_directory(p))
        {
            explore_directory(p);
            continue;
        }

        m_pathes.push_back(p);
    }
}

std::filesystem::path PathExplorer::guess_last_backup() const
{
    std::vector<std::filesystem::path> directories;

    auto root_backup = m_config->get_real_destination_directory();
    for (const std::filesystem::path & path : std::filesystem::directory_iterator{root_backup})
    {   
        directories.push_back(path);
    }

    std::sort(directories.begin(), directories.end(), [](auto& s1, auto& s2) {
        return s1 > s2;
    });

    return directories.at(0);
}
