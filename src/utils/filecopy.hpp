#ifndef FILE_COPY_HPP
#define FILE_COPY_HPP

#include "../config.hpp"

#include <filesystem>
#include <memory>

class FileCopy
{
public:
    enum class Status {
        Success, Fail
    };


    FileCopy(std::shared_ptr<Config_t> config) : m_config(config) {}
    
    void copy_file(const std::filesystem::path &source, const std::filesystem::path &dest);
    Status create_parent_directories(const std::filesystem::path& source);
    void effectiveCopy(const std::filesystem::path &source, const std::filesystem::path &dest);
private:
    std::shared_ptr<Config_t> m_config;
};

#endif