#include "filecopy.hpp"
#include "destination_directory.hpp"

#include <iostream>

namespace utils
{
    void FileCopy::copy_file(const std::filesystem::path &source, const std::filesystem::path &dest) {
        const auto dest_file_path = Config::instance()->get_destination_directory(source);
        const auto dest_parent = dest_file_path.parent_path();

        if (auto success = DestinationDirectory::create_if_not_exists(dest_parent); success == Status::Success)
        {
            effective_copy(source, dest_file_path);
        }
    }

    void FileCopy::effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest)
    {
        if (Config::instance()->backup.dry_run)
        {
            return;
        }

        try
        {
            std::filesystem::copy_file(source, dest);
        }
        catch (const std::filesystem::filesystem_error &err)
        {
            std::cerr << "Unable to copy " << source
                      << " to " << dest
                      << " because: " << err.what() << std::endl;
        }
    }
}
