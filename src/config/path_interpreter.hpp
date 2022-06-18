//
// Created by regis on 18/06/2022.
//

#ifndef R_BACKUP_PATH_INTERPRETER_HPP
#define R_BACKUP_PATH_INTERPRETER_HPP


#include <filesystem>

class PathInterpreter
{
public:
    explicit PathInterpreter(const std::filesystem::path &p);

    std::filesystem::path normalize_path();

private:
    std::filesystem::path path;
};


#endif //R_BACKUP_PATH_INTERPRETER_HPP
