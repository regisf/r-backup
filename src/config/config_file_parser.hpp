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

#ifndef CONFIG_FILE_PARSER_HPP
#define CONFIG_FILE_PARSER_HPP

#include "../utils/dependencies.hpp"
#include "config.hpp"

#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <memory>

#include <yaml-cpp/yaml.h>


class ConfigFileParser
{
public:
    explicit ConfigFileParser() = default;

    /**
     * @brief Convert the configuration file into a configuraiton object
     * 
     * @return std::shared_ptr<Config> The configuration object
     */
    std::shared_ptr<Config> to_config();

    /**
     * @brief Read the configuration file as given. If the file exists, the method parse it if not
     * read the default config file
     * 
     * @param file_path The config file to read and parser
     * @return This parser object
     */
    static std::shared_ptr<ConfigFileParser> read_default_config_file(const std::string &file_path = "");

    /**
     * @brief Parse the configuration file. If there's a YAML syntax error, exit with status == 1
     *        else continue anyway.
     * 
     * @param path
     * @path exit_cb Here for testing reasons
     */
    void parse_file(const std::string &path, exit_callback exit_cb = &std::exit);


private:
    void extract_destination_path();

    void extract_root_path();

    void extract_exclusions();

    void extract_inclusions();

    void extract_excluded_paths(const YAML::Node &excluded);

    void extract_excluded_patterns(const YAML::Node &excluded);

private:
    std::set<std::string> m_include_directories;
    std::vector<std::regex> m_exclude_patterns;
    std::set<std::string> m_exclude_paths;
    YAML::Node m_config_node;
    std::string m_root;
    std::filesystem::path m_destination;
};

#endif
