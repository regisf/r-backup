#include "backup_command_line.hpp"
#include "command_line.hpp"

#include <iostream>
#include <sstream>

const std::string USAGE = R"(
r-backup backup [OPTIONS]

The options given override the default configuration file.
Options are:
    --config-file: set the config file that contains
    --destination: Set the destionation
    --backup-directory-name: Set the destination name.
                             Will be [Destination Dir]/[Backup Directory Name]/[DateForBackup]
                             Default is : backup
    --strategy: The default backup strategy.
        Strategies are
            keep-all: Keep all backups. You remove old ones manually
            keep-nth: Keep only nth last backup. Implies --nth argument
            remove-previous:  Keep only one backup.
            difference: Backup only the last modified file
    --nth: The number of backup to keep (implies --strategy keep_nth, exclude all other strategies)
           Older backups will be destroyed.

)";

#define EXPECT_NEXT_ARG(a, b, c) \
    if ((a + 1) == b) { \
        throw CommandLineError("Error: --##c option need an extra argument"); \
    }


BackupCommandLine::BackupCommandLine(const std::vector<std::string> & args) : args(args)
{

}

void BackupCommandLine::parse(exit_callback exit_cb)
{
    bool unknown_option{false};

    for (int i = 1, l = args.size(); i < l; ++i)
    {
        const std::string argument{args.at(i)};

        if (!argument.compare("--help") || !argument.compare("-h"))
        {
            std::cerr << USAGE;
            exit_cb(0);
            unknown_option = false; // To avoid failure on testing
        }

        else if (!argument.compare("--strategy"))
        {
            EXPECT_NEXT_ARG(i, l, strategy)
            strategy = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--nth"))
        {
            EXPECT_NEXT_ARG(i, l, nth)
            std::stringstream ss;
            ss << args.at(++i);
            ss >> nth;
        }

        else if (!argument.compare("--dry-run"))
        {
            dry_run = true;
            unknown_option = false;
        }

        else if (!argument.compare("--config-file"))
        {

            EXPECT_NEXT_ARG(i, l, config-file)
            config_file = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--destination"))
        {
            EXPECT_NEXT_ARG(i, l, destination)
            destination = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--backup-directory-name"))
        {
            EXPECT_NEXT_ARG(i,l, backup-directory-name)
            backup_dir_name = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--verbose"))
        {
            verbose = true;
            unknown_option = false;
        }

        else
        {
            unknown_option = true;
            break;
        }
    }

    if (unknown_option)
    {
        throw CommandLineError("Unknown argument for backup action");
    }
}

std::string BackupCommandLine::get_strategy() const
{
   return strategy;
}

int BackupCommandLine::get_nth() const
{
    return nth;
}

bool BackupCommandLine::get_dry_run() const
{
    return dry_run;
}

std::filesystem::path BackupCommandLine::get_config_file() const
{
    return config_file;
}

std::filesystem::path BackupCommandLine::get_destination() const
{
    return destination;
}

std::filesystem::path BackupCommandLine::get_backup_directory_name() const
{
    return backup_dir_name;
}

bool BackupCommandLine::get_verbose() const
{
    return verbose;
}
