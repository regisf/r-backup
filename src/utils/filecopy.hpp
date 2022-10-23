#ifndef FILE_COPY_HPP
#define FILE_COPY_HPP

#include "../config/config.hpp"
#include "enums.hpp"

#include <filesystem>
#include <memory>
#include <utility>

namespace utils
{
    class FileCopy
    {
    public:
        static void copy_file(const std::filesystem::path &source);
    };
}
#endif
