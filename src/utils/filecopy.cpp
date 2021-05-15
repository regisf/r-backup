#include "filecopy.hpp"

#include <iostream>

void FileCopy::copy_file(const std::filesystem::path &source, const std::filesystem::path &dest)
{
    const auto dest_file_path = m_config->get_destination_directory(source);
    const auto dest_parent = dest_file_path.parent_path();

    if (!m_config->is_destination_dir_exists(source))
    {
        auto status = create_parent_directories(dest_parent);
        if (status != Status::Success)
        {
            return;
        }
    }

    effectiveCopy(source, dest_file_path);
}

FileCopy::Status FileCopy::create_parent_directories(const std::filesystem::path &dest_parent)
{
    try
    {
        std::filesystem::create_directories(dest_parent);
    }
    catch (std::filesystem::filesystem_error &err)
    {
        std::cerr << "Unable to create the directory '" << dest_parent << "' because: " << err.what() << "\n";
        return Status::Fail;
    }

    return Status::Success;
}

void FileCopy::effectiveCopy(const std::filesystem::path &source, const std::filesystem::path &dest)
{
    try
    {
        std::filesystem::copy_file(source, dest);
    }
    catch (std::filesystem::filesystem_error &err)
    {
        std::cerr << "Unable to copy " << source
                  << " to " << dest
                  << " because: " << err.what() << std::endl;
    }
}