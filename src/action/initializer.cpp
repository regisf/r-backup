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

#include "initializer.hpp"
#include "../command_line.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace action
{
    static void display_banner()
    {
        std::cout << R"(
        _____        ____             _                
        |  __ \      |  _ \           | |               
        | |__) |_____| |_) | __ _  ___| | ___   _ _ __  
        |  _  /______|  _ < / _` |/ __| |/ / | | | '_ \
        | | \ \      | |_) | (_| | (__|   <| |_| | |_) |
        |_|  \_\     |____/ \__,_|\___|_|\_\\__,_| .__/ 
                                                 | |    
                                                 |_|

)";
    }

    static std::filesystem::path config_file_path()
    {
        const std::filesystem::path home_path{std::getenv("HOME")};
        const auto config_path = home_path / ".config" / "r-backup" / "config.yml";

        return config_path;
    }

    static bool install_config_file()
    {
        bool should_continue{true};

        std::cout << "Install configuration file ~/.config/r-backup/config.yml\n";

        const std::filesystem::path config_path = config_file_path();

        if (std::filesystem::exists(config_path))
        {
            std::cout << "Warning: The configuration file already exists. Nothing do do.\n";
            should_continue = false;
        }

        return should_continue;
    }

    static std::string ask_root_directory()
    {
        std::string root_path;
        std::string default_path{std::getenv("HOME")};

        std::cout << "1. ";
        while (true)
        {
            std::cout << "Please enter the root directory path to backup [default: " << default_path << "]: ";
            std::getline(std::cin, root_path);
            if (root_path.empty())
            {
                root_path = default_path;
            }

            if (std::filesystem::is_directory(root_path))
            {
                break;
            }

            std::cout << "Error: The directory " << root_path << " doesn't exist or isn't a directory.\n";
        }

        return root_path;
    }

    static std::string ask_destination()
    {
        std::string destination_path;

        std::cout << "2. ";
        while (true)
        {
            std::cout << "Please enter the destination directory path to backup on: ";
            std::getline(std::cin, destination_path);

            if (std::filesystem::is_directory(destination_path))
            {
                break;
            }

            std::cout << "Error: The directory " << destination_path << " doesn't exist or isn't a directory.\n";
        }

        return destination_path;
    }

    static std::string ask_target_name()
    {
        std::string target_name;

        std::cout << "3. Please enter the target name [default: r-backup]: ";
        std::getline(std::cin, target_name);
        
        if (target_name.empty())
        {
            target_name = "r-backup";
        }

        return target_name;
    }

    static void save_config_file(const std::string &root_path, const std::string &destination, const std::string &target, const std::string &periodicity)
    {
        auto config_path = config_file_path();
        auto config_path_str = config_path.string();
        auto config_dir_path = config_path.parent_path();

        if (!std::filesystem::is_directory(config_dir_path)) 
        {
            std::cout << "Creating the directory " << config_dir_path.string() << "\n";
        }

        std::stringstream ss;
        
        ss << "root: " << root_path << "\n"
           << "destination: " << destination << "\n"
           << "name: " << target << "\n"
           << "periodicity: " << periodicity << "\n"
           << "exclude:\n"
           << "  patterns: []\n"
           << "  paths: []\n"
           << "include: []\n";

        std::ofstream config_file{config_path, std::ofstream::out};
        config_file << ss.str();
        config_file.close();
        
        std::cout << "Config file wrote here : " << config_path << "\n";
    }

    static std::string ask_periodicity()
    {
        std::string periodicity;

        std::cout << "4. Please enter periodicity (number followed by time unit: minute, hour, day, week, month) [default: 1 day]: ";

        while (true)
        {
            std::getline(std::cin, periodicity);
            
            if (periodicity.empty())
            {
                periodicity = "1 day";
            }

            std::regex regex{"^\\d+\\s+(minute|hour|day|week|month)[s]*$"};
            const auto match = std::regex_match(periodicity, regex);
            if (match)
            {
                break;
            }

            std::cout << "Error: " << periodicity << " couldn't be parsed\n";
        }
        
        return periodicity;
    }
    
    void initialize()
    {
        display_banner();

        const bool should_continue = install_config_file();
        if (!should_continue)
        {
            return;
        }

        std::string root_path = ask_root_directory();
        std::string destination_path = ask_destination();
        std::string target_name = ask_target_name();
        std::string periodicity = ask_periodicity();

        save_config_file(root_path, destination_path, target_name, periodicity);
    }
}