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

#include <algorithm>
#include <regex>
#include <iostream>


std::set<std::filesystem::path> PathExplorer::explore()
{
    auto directories = Config::instance()->include_directories;

    // auto last_backup_date = guess_last_backup();

    // No included directory so use backup root directory
    if (directories.empty())
    {
        directories.insert(Config::instance()->root_path);
    }

    std::for_each(directories.begin(),
                  directories.end(),
                  [&](const auto &path) {
                      explore_directory(path);
                  });

    return m_pathes;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

bool PathExplorer::is_pattern_match(const std::filesystem::path &p) const
{
    const auto pstr = p.string();
    return std::ranges::any_of(Config::instance()->exclusion_patterns.begin(),
                               Config::instance()->exclusion_patterns.end(),
                               [&pstr](const auto &p) {
                                   return std::regex_search(pstr, std::regex{p});
                               });
}

#pragma clang diagnostic pop

bool PathExplorer::is_in_exclusion_path(const std::filesystem::path &p) const
{
    const auto pstr = p.string();
    return std::ranges::any_of(Config::instance()->exclusion_paths.begin(),
                               Config::instance()->exclusion_paths.end(),
                               [&pstr](const auto &path) {
                                   return pstr.starts_with(path);
                               });
}

bool PathExplorer::should_be_skipped(const std::filesystem::path &p,
                                     IsSymlinkFunc is_symlink) const
{
    return is_symlink(p) || is_pattern_match(p) || is_in_exclusion_path(p);
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
        m_pathes.insert(dir_path);
        return;
    }

    for (auto &p: std::filesystem::directory_iterator(dir_path))
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

        m_pathes.insert(p);
    }
}

std::filesystem::path PathExplorer::guess_last_backup() const
{
    std::vector<std::filesystem::path> directories;

    auto root_backup = Config::instance()->get_real_destination_directory();

    for (const std::filesystem::path &path: std::filesystem::directory_iterator{root_backup})
    {
        directories.push_back(path);
    }

    std::sort(directories.begin(),
              directories.end(),
              [](auto &s1, auto &s2) {
                  return s1 > s2;
              });

    return directories.at(0);
}
