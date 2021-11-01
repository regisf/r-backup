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

#include "config_file.hpp"

#include <iostream>
#include <cstdlib>
#include <boost/algorithm/string/predicate.hpp>

#include <yaml-cpp/yaml.h>

static const std::string SystemSeparator{"/"};

void ConfigFile::parse_file(const std::string &path)
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
        std::cerr << "Error : " << err.what() << "\nContinue anyway. \n";
        return;
    }

    extract_destination_path();
    extract_root_path();
    extract_exclusions();
    extract_inclusions();
}

void ConfigFile::extract_destination_path()
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

void ConfigFile::extract_root_path()
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
    if (!boost::algorithm::ends_with(m_root, SystemSeparator))
    {
        m_root += SystemSeparator;
    }
}

void ConfigFile::extract_exclusions()
{

    const auto &excluded = m_config["exclude"];
    if (excluded.IsDefined())
    {
        extract_excluded_patterns(excluded);
        extract_excluded_paths(excluded);
    }
}

void ConfigFile::extract_excluded_paths(const YAML::Node &excluded)
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

void ConfigFile::extract_excluded_patterns(const YAML::Node &excluded)
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

void ConfigFile::extract_inclusions()
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

std::vector<std::string> &ConfigFile::get_excluded_paths()
{
    return m_exclude_paths;
}

std::vector<std::regex> &ConfigFile::get_excluded_patterns()
{
    return m_exclude_patterns;
}

std::vector<std::string> &ConfigFile::get_included_directories()
{
    return m_include_directories;
}

std::string ConfigFile::get_root_path() const
{
    return m_root;
}

std::filesystem::path ConfigFile::get_destination() const
{
    return m_destination;
}

std::vector<std::string> ConfigFile::get_paths_to_explore()
{
    std::vector<std::string> paths;

    if (m_include_directories.size())
    {
        const auto sep = boost::algorithm::ends_with(get_root_path(), "/") ? "" : "/";

        std::for_each(m_include_directories.begin(), m_include_directories.end(), [&](const auto &p) {
            paths.push_back(get_root_path() + sep + p);
        });
    }

    else
    {
        paths.push_back(get_root_path());
    }

    return paths;
}

void ConfigFile::set_destination(const std::filesystem::path &path)
{
    m_destination = path;
}

void ConfigFile::read_default_config_file()
{
    const std::string home_path{std::getenv("HOME")};
    std::filesystem::path config_file = std::filesystem::path(home_path) /
                                        ".config" / "r-backup" / "config.yml";

    if (std::filesystem::exists(config_file))
    {
        parse_file(config_file);
    }
}
