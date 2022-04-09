#ifndef BACKUPCOMMANDLINE_HPP
#define BACKUPCOMMANDLINE_HPP

#include <vector>
#include <string>
#include <filesystem>

using exit_callback = void(*)(int);

class IBackupCommandLine
{
public:
    virtual void parse(exit_callback exit_cb) = 0;
    virtual std::string get_strategy() const = 0;
    virtual int get_nth() const = 0;
    virtual bool get_dry_run() const = 0;
    virtual std::filesystem::path get_config_file() const = 0;
    virtual std::filesystem::path get_destination() const = 0;
    virtual std::filesystem::path get_backup_directory_name() const = 0;
    virtual bool get_verbose() const = 0;
};

class BackupCommandLine : private IBackupCommandLine
{
public:
    explicit BackupCommandLine(const std::vector<std::string> & args);

    void parse(exit_callback exit_cb = &std::exit) override;

    std::string get_strategy() const override;

    int get_nth() const override;

    bool get_dry_run() const override;

    std::filesystem::path get_config_file() const override;

    std::filesystem::path get_destination() const override;

    std::filesystem::path get_backup_directory_name() const override;

    bool get_verbose() const override;

private:
    std::vector<std::string> args;

    std::string strategy;

    int nth{-1};

    bool dry_run{false};

    bool verbose{false};

    std::filesystem::path config_file;

    std::filesystem::path destination;

    std::filesystem::path backup_dir_name;
};

#endif // BACKUPCOMMANDLINE_HPP
