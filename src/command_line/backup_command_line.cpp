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

BackupCommandLineOptions BackupCommandLine::parse(exit_callback exit_cb)
{
    bool unknown_option{false};

    for (int i = 1, l = args.size(); i < l; ++i)
    {
        const std::string argument{args.at(i)};

        if (!(argument.compare("help") && argument.compare("--help") && argument.compare("-h")))
        {
            std::cerr << USAGE;
            exit_cb(0);
            unknown_option = false; // To avoid failure on testing
        }

        else if (!argument.compare("--strategy"))
        {
            EXPECT_NEXT_ARG(i, l, strategy)
            options.strategy = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--nth"))
        {
            EXPECT_NEXT_ARG(i, l, nth)
            std::stringstream ss;
            ss << args.at(++i);
            ss >> options.nth;
        }

        else if (!argument.compare("--dry-run"))
        {
            options.dry_run = true;
            unknown_option = false;
        }

        else if (!argument.compare("--config-file"))
        {

            EXPECT_NEXT_ARG(i, l, config-file)
            options.config_file = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--destination"))
        {
            EXPECT_NEXT_ARG(i, l, destination)
            options.destination = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--backup-directory-name"))
        {
            EXPECT_NEXT_ARG(i,l, backup-directory-name)
            options.backup_dir_name = args.at(++i);
            unknown_option = false;
        }

        else if (!argument.compare("--verbose"))
        {
            options.verbose = true;
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

    return options;
}
