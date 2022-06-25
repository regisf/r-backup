//
// Created by regis on 25/06/2022.
//

#ifndef R_BACKUP_BACKUP_ERROR_HPP
#define R_BACKUP_BACKUP_ERROR_HPP

#include <exception>
#include <string>

class BackupError : public std::exception
{
public:
    explicit BackupError(const std::string &msg) : message(msg)
    {}

    [[nodiscard]] const char *what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

#endif //R_BACKUP_BACKUP_ERROR_HPP
