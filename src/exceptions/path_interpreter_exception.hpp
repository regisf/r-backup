//
// Created by regis on 18/06/2022.
//

#ifndef R_BACKUP_PATH_INTERPRETER_EXCEPTION_HPP
#define R_BACKUP_PATH_INTERPRETER_EXCEPTION_HPP


class PathInterpreterException : public std::exception
{
public:
    explicit PathInterpreterException(const char *msg) : _what(msg)
    {}

    [[nodiscard]] const char *what() const noexcept override
    { return _what; }

private:
    const char *_what{nullptr};
};


#endif //R_BACKUP_PATH_INTERPRETER_EXCEPTION_HPP
