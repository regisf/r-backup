#include "filecopy.hpp"

#include <iostream>

void FileCopy::copy_file(const std::filesystem::path &source, const std::filesystem::path &dest) {
    const auto dest_file_path = m_config->get_destination_directory(source);
    const auto dest_parent = dest_file_path.parent_path();

    if (!m_config->is_destination_dir_exists(dest))
    {
        auto status = create_parent_directories(dest_parent);
        if (status != Status::Success)
        {
            return;
        }
    }

    effective_copy(source, dest_file_path);
}

Status FileCopy::create_parent_directories(const std::filesystem::path &dest_parent)
{
    if (m_config->backup.dry_run)
    {
        std::cout << "Creating directory \"" << dest_parent.string() << "\"\n";
    }
    else 
    {
        try
        {
            std::filesystem::create_directories(dest_parent);
        }
        catch (const std::filesystem::filesystem_error &err)
        {
            std::cerr << "Unable to create the directory '" << dest_parent << "' because: " << err.what() << "\n";
            return Status::Fail;
        }
    }
    return Status::Success;
}

void FileCopy::effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest) 
{
    if (m_config->backup.dry_run)
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

void FileCopy::set_config(std::shared_ptr<Config> config)
{
    m_config = config;
}
