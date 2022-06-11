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

#include <iostream>
#include <filesystem>


namespace action
{
    void Backup::start()
    {
        if (action::Backup bk_act; bk_act.can_backup())
        {
            std::set<std::filesystem::path> pathes;

            if (!Config::instance()->is_backup_exists())
            {
                auto c = Config::instance();
                std::cout << "Exploring directory " << Config::instance()->root_path << "\n";
                PathExplorer explorer;
                pathes = explorer.explore();
                std::cout << "Got " << pathes.size() << " files found\n";
            }
            else
            {
                std::cout << "There's nothing to backup because it is already backuped.\n";
            }

            bk_act.backup(pathes);
        }
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

    void Backup::backup(const std::set<std::filesystem::path> &paths)
    {
        if (!paths.empty())
        {
            std::cout << "Got " << paths.size() << " files to backup\n";
        }
        else
        {
            std::cout << "Nothing to backup\n";
        }

        const auto destination = Config::instance()->backup.destination;
        for (const auto &path: paths)
        {
            if (Config::instance()->backup.verbose)
            {
                std::cout << "Copying " << path.string() << " to " << (destination / path.filename()).string() << "\n";
            }

            utils::FileCopy::copy_file(path, destination);
        }
    }

#pragma clang diagnostic pop

    bool Backup::can_backup()
    {
        if (Config::instance()->backup.dry_run)
        {
            return true;
        }

        bool status = true;

        if (!std::filesystem::exists(Config::instance()->backup.destination))
        {
            std::stringstream ss;
            ss << "Unable to find the destination directory:"
               << Config::instance()->backup.destination;

            status = false;
            m_error_msg = ss.str();
        }

        return status;
    }

    std::string Backup::error_message() const
    { return m_error_msg; }
}
