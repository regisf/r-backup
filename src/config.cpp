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
#include "date.hpp"

#include <filesystem>
#include <iostream>

std::filesystem::path Config::get_() const
{
    return configFile.get_destination() / backup.backup_dir_name;
}

std::filesystem::path Config::get_destination_directory() const
{
    return get_() / date::get_now();
}

bool Config::backup_exists() const
{
    return std::filesystem::exists(get_destination_directory());
}

std::filesystem::path Config::remove_root_path(const std::filesystem::path &path) const
{
    const auto root = configFile.get_root_path();
    const auto path_str = path.string();
    std::size_t root_size = root.size();

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
    const auto parent = remove_root_path(parent_path);

    return get_destination_directory() / parent / path.filename();
}

bool Config::is_destination_dir_exists(const std::filesystem::path &source) const
{
    const auto dest_parent = get_destination_directory(source).parent_path();

    return std::filesystem::exists(dest_parent) && std::filesystem::is_directory(dest_parent);
}
