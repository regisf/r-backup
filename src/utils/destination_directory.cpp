#include "destination_directory.hpp"

#include "../config/config.hpp"
#include <iostream>

namespace utils
{

    Status create_parent_directories(const std::filesystem::path &dest)
    {
        if (Config::instance()->backup.dry_run)
        {
            std::cout << "Creating directory \"" << dest.string() << "\"\n";
            return Status::Success;
        }

        Status result{Status::Fail};

        try
        {
            std::filesystem::create_directories(dest);
            result = Status::Success;
        }
        catch (const std::filesystem::filesystem_error &err)
        {
            std::cerr << "Unable to create the directory '" << dest << "' because: " << err.what() << "\n";
        }

        return result;
    }

    Status DestinationDirectory::create_if_not_exists(const std::filesystem::path & path)
    {
        Status result{Status::Fail};

        if (!std::filesystem::exists(path))
        {
            result = create_parent_directories(path);
        }

        return result;
    }
}
