#ifndef BACKUPCOMMANDLINE_HPP
#define BACKUPCOMMANDLINE_HPP

#include <vector>
#include <string>
#include <filesystem>
#include "../utils/dependencies.hpp"


struct BackupCommandLineOptions
{
    std::string strategy;

    int nth{-1};

    bool dry_run{false};

    bool verbose{false};

    std::filesystem::path config_file;

    std::filesystem::path destination;

    std::filesystem::path backup_dir_name;
};

class BackupCommandLine
{
public:
    explicit BackupCommandLine(std::vector<std::string> args);

    BackupCommandLineOptions parse(exit_callback exit_cb = &std::exit);

private:
    std::vector<std::string> args;
    BackupCommandLineOptions options;
};

#endif // BACKUPCOMMANDLINE_HPP
