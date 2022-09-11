//
// Created by regis on 18/06/2022.
//

#include "path_interpreter.hpp"
#include "../exceptions/path_interpreter_exception.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"

static std::string replace_home(const std::string &path)
{
    std::string p{path};

    if (auto pos = p.find("$HOME");
            pos != std::string::npos && pos > 0)
    {
        throw PathInterpreterException("$HOME variable must be at the beginning of the path");
    }

    else if (p.starts_with("$HOME"))
    {
        auto home = std::string{std::getenv("HOME")};
        p.replace(0, 5, home);
    }

    return p;
}

static std::string replace_pwd(const std::string &path)
{
    std::string p{path};
    auto pos_pwd = p.find("$PWD");
    if (pos_pwd != std::string::npos)
    {
        auto pwd = std::filesystem::current_path();
        p.replace(pos_pwd, 4, pwd);
    }

    return p;
}

PathInterpreter::PathInterpreter(const std::filesystem::path &p)
        : path(p)
{
}

#pragma clang diagnostic pop

std::filesystem::path PathInterpreter::normalize_path()
{
    std::string home_replace = replace_home(path.string());
    std::string pwd_replace = replace_pwd(home_replace);

    return std::filesystem::path{pwd_replace};
}
