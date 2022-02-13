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

enum class CommandLineType
{
    Unknown,
    Help,
    Init,
    Backup,
    Restore,
    Start,
    Stop
};

class IConfig
{
public:
    virtual std::filesystem::path get_real_destination_directory() const = 0;
    virtual std::filesystem::path get_destination_directory() const = 0;
    virtual bool is_backup_exists() const = 0;
    virtual std::filesystem::path get_destination_directory(const std::filesystem::path &path) const = 0;
    virtual bool is_destination_dir_exists(const std::filesystem::path &source) const = 0;
};


/**
 * @brief Configuration container
 */
struct Config : IConfig
{
    enum class Strategy
    {
        NotSet,
        KeepAll,
        KeepNth,
        RemovePrevious,
        Difference
    };

    bool verbose{false};
    bool dry_run{false};

    Strategy strategy{Strategy::NotSet};
    CommandLineType action{CommandLineType::Unknown};
    ConfigFile configFile;


    std::string backup_dir_name{};

    /**
     * @brief Get the destination directory
     *
     * @return std::filesystem::path
     */
    std::filesystem::path get_destination_directory() const override;

    /**
     * @brief Get the destination directory object
     *
     * @param path
     * @return std::filesystem::path
     */
    std::filesystem::path get_destination_directory(const std::filesystem::path &path) const override;

    /**
     * @brief Test if the destination directory exists
     *
     * @return true If exists
     * @return false if not exists
     */
    bool is_backup_exists() const override;

    /**
     * @brief Test if the destination directory exists
     *
     * @param source The source path
     * @return true If the directory exists
     * @return false if the directory doesn't exist
     */
    bool is_destination_dir_exists(const std::filesystem::path &source) const override;

    /**
     * @brief Get the real destination directory object
     *
     * @return std::filesystem::path
     */
    std::filesystem::path get_real_destination_directory() const override;

private:
    /**
     * @brief Extract path without the root path
     *
     * @param path The source path
     * @return std::filesystem::path The path without the base path
     */
    std::filesystem::path extract_from_root_path(const std::filesystem::path &path) const;
};

#endif
