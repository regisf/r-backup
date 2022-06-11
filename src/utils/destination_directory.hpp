#ifndef DESTINATIONDIRECTORY_HPP
#define DESTINATIONDIRECTORY_HPP

#include "enums.hpp"

#include <filesystem>

namespace utils
{
    class DestinationDirectory
    {
    public:
        static Status create_if_not_exists(const std::filesystem::path &path);
    };
}
#endif // DESTINATIONDIRECTORY_HPP
