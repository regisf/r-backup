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

#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

#include <string>
#include <vector>
#include <regex>
#include <filesystem>

#include <yaml-cpp/yaml.h>

class ConfigFile
{
public:
    explicit ConfigFile() = default;

    void read_default_config_file();
    void parse_file(const std::string &path);
    std::vector<std::string> &get_excluded_paths();
    std::vector<std::regex> &get_excluded_patterns();
    std::vector<std::string> &get_included_directories();
    std::vector<std::string> get_paths_to_explore();
    std::string get_root_path() const;
    std::filesystem::path get_destination() const;
    void set_destination(const std::filesystem::path& path);
    
private:
    void extract_destination_path();
    void extract_root_path();
    void extract_exclusions();
    void extract_inclusions();
    void extract_excluded_paths(const YAML::Node &excluded);
    void extract_excluded_patterns(const YAML::Node &excluded);

private:
    std::vector<std::string> m_include_directories;
    std::vector<std::regex> m_exclude_patterns;
    std::vector<std::string> m_exclude_paths;
    YAML::Node m_config;
    std::string m_root;
    std::filesystem::path m_destination;
};

#endif
