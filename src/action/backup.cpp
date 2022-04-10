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

#include "backup.hpp"
#include "path_explorer.hpp"

#include "../command_line/command_line.hpp"
#include "../config_file_parser.hpp"
#include "../date.hpp"

#include <iostream>
#include <filesystem>
#include <utility>

#include <yaml-cpp/yaml.h>


namespace action {
    Backup::Backup(std::shared_ptr<Config> config, FileCopy copy)
            : m_config(std::move(config)), m_file_copy(std::move(copy)) {
    }

    void Backup::backup(const std::vector<std::filesystem::path> &pathes) {
        if (!pathes.empty()) {
            std::cout << "Got " << pathes.size() << " files to backup\n";
        } else {
            std::cout << "Nothing to backup\n";
        }

        const auto destination = m_config->backup.destination;
        for (const auto &path : pathes) {
            if (m_config->backup.verbose) {
                std::cout << "Copying " << path.string() << " to " << (destination / path.filename()).string() << "\n";
            }

            m_file_copy.copy_file(path, destination);
        }
    }

    bool Backup::can_backup() {
        if (m_config->backup.dry_run) {
            return true;
        }

        bool status = true;

        if (!std::filesystem::exists(m_config->backup.destination)) {
            std::stringstream ss;
            ss << "Unable to find the destination directory:"
               << m_config->backup.destination;

            status = false;
            m_error_msg = ss.str();
        }

        return status;
    }

    std::string Backup::error_message() const { return m_error_msg; }
}
