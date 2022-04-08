#ifndef BACKUPCOMMANDLINE_HPP
#define BACKUPCOMMANDLINE_HPP

#include <vector>
#include <string>

using exit_callback = void(*)(int);

class IBackupCommandLine
{
public:
    virtual void parse(exit_callback exit_cb) = 0;
    virtual std::string get_strategy() const = 0;
    virtual int get_nth() const = 0;
};

class BackupCommandLine : private IBackupCommandLine
{
public:
    explicit BackupCommandLine(const std::vector<std::string> & args);

    void parse(exit_callback exit_cb = &std::exit) override;

    std::string get_strategy() const override;

    int get_nth() const override;

private:
    std::vector<std::string> args;

    std::string strategy;

    int nth;

};

#endif // BACKUPCOMMANDLINE_HPP
