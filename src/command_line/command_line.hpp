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

#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include "../config.hpp"

#include <vector>
#include <string>
#include <memory>

class BackupCommandLineOptions;

class CommandLineError : public std::exception
{
public:
    std::string message;

    CommandLineError(const std::string& msg = "")
        : message{msg}    
        {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class ICommandLine
{
public:
    virtual std::shared_ptr<Config> parse() = 0;
    virtual std::vector<std::string> get_args() noexcept = 0;
};

class CommandLine : public ICommandLine
{
public:
    /**
     * @brief CommandLine
     * @param argc The number of arguments
     * @param argv A list of arguments
     */
    explicit CommandLine(int argc, char **argv);

    /**
     * @brief Parses the command line, and returns the configuration
     * @return the configuration object
     */
    std::shared_ptr<Config> parse() override;

    /**
     * @brief Get the currated list of argument
     * @return The command line argument
     */
    std::vector<std::string> get_args() noexcept override;

private:
    std::filesystem::path resolve_path(const std::string &path) const;
    BackupCommandLineOptions backup_configuration();
    void init_configuration();
    void restore_configuration();

private:
    std::shared_ptr<Config> m_config;
    std::vector<std::string> args;
};

#endif
