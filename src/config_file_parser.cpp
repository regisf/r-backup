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
#include "config_file_parser.hpp"

#include <iostream>
#include <cstdlib>
#include <memory>

#include <yaml-cpp/yaml.h>

#define SystemSeparator std::string{"/"}

std::shared_ptr<Config> ConfigFileParser::to_config() 
{
    auto config = std::make_shared<Config>();
    config->root_path = m_root;
    config->backup.destination = m_destination;
    config->exclusion_paths = m_exclude_paths;
    config->exclusion_patterns = m_exclude_patterns;
    config->include_directories = m_include_directories;

    return config;

}

void ConfigFileParser::parse_file(const std::string &path)
{
    try
    {
        m_config = YAML::LoadFile(path);
    }
    catch (YAML::ParserException &err)
    {
        std::cerr << "Error while parsing : " << err.what() << "\n";
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception &err)
    {
        std::cerr << "Error : " << err.what() << "\nContinue anyway. \n";
        return;
    }

    extract_destination_path();
    extract_root_path();
    extract_exclusions();
    extract_inclusions();
}

void ConfigFileParser::extract_destination_path()
{
    const auto destination = m_config["destination"];
    const auto name = m_config["name"];

    if (destination.IsDefined())
    {
        std::filesystem::path dest_path{destination.as<std::string>()};

        if (name.IsDefined())
        {
            dest_path /= std::filesystem::path{name.as<std::string>()};
        }

        m_destination = dest_path;
    }
}

void ConfigFileParser::extract_root_path()
{
    const auto root = m_config["root"];
    if (root.IsDefined())
    {
        m_root = root.as<std::string>();
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home == nullptr)
        {
            std::cerr << "Error: Unable to get HOME environment variable. I think you've got a serious problem" << std::endl;
            std::exit(1);
        }

        m_root = std::string{home};
    }

    // Ensure path
    if (!m_root.ends_with(SystemSeparator))
    {
        m_root += SystemSeparator;
    }
}

void ConfigFileParser::extract_exclusions()
{

    const auto &excluded = m_config["exclude"];
    if (excluded.IsDefined())
    {
        extract_excluded_patterns(excluded);
        extract_excluded_paths(excluded);
    }
}

void ConfigFileParser::extract_excluded_paths(const YAML::Node &excluded)
{
    const auto &pathes = excluded["paths"];
    if (pathes.IsDefined())
    {
        for (const auto &path : pathes)
        {
            m_exclude_paths.push_back(m_root + path.as<std::string>());
        }
    }
}

void ConfigFileParser::extract_excluded_patterns(const YAML::Node &excluded)
{
    const auto &patterns = excluded["patterns"];
    if (patterns.IsDefined())
    {
        for (const auto &pattern : patterns)
        {
            const auto p = pattern.as<std::string>();

            try
            {
                m_exclude_patterns.push_back(std::regex{p});
            }
            catch (std::regex_error &err)
            {
                std::cerr << "Regex error: pattern \"" << p << "\" is not an understandable regular expression." << std::endl;
            }
        }
    }
}

void ConfigFileParser::extract_inclusions()
{
    const auto &included = m_config["include"];
    if (included.IsDefined())
    {
        for (const auto &include : included)
        {
            m_include_directories.push_back(m_root + include.as<std::string>());
        }
    }
}

std::vector<std::string> ConfigFileParser::get_paths_to_explore()
{
    std::vector<std::string> paths;

    if (m_include_directories.size())
    {
        const auto sep = m_root.ends_with("/") ? "" : "/";

        std::for_each(m_include_directories.begin(), m_include_directories.end(), [&](const auto &p) {
            paths.push_back(m_root + sep + p);
        });
    }

    else
    {
        paths.push_back(m_root);
    }

    return paths;
}

std::shared_ptr<ConfigFileParser> ConfigFileParser::read_default_config_file(const std::string & file_path)
{
    auto parser = std::make_shared<ConfigFileParser>();
    std::filesystem::path config_file_path;

    if (file_path.empty()) {
        const std::string home_path{std::getenv("HOME")};
        config_file_path = std::filesystem::path(home_path) / ".config" / "r-backup" / "config.yml";
    } else {
        config_file_path = file_path;
    }

    if (std::filesystem::exists(config_file_path))
    {
        parser->parse_file(config_file_path);
    }

    return parser;
}
