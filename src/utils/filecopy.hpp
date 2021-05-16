#ifndef FILE_COPY_HPP
#define FILE_COPY_HPP

#include "../config.hpp"

#include <filesystem>
#include <memory>

enum class Status
{
    Success,
    Fail
};

class IFileCopy
{
public:
    virtual void copy_file(const std::filesystem::path &source, const std::filesystem::path &dest) = 0;
    virtual Status create_parent_directories(const std::filesystem::path &source) = 0;
    virtual void effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest) = 0;
};

class FileCopy 
{
public:
    FileCopy(std::shared_ptr<IConfig> config) : m_config(config) {}

    void copy_file(const std::filesystem::path &source, const std::filesystem::path &dest);
    virtual Status create_parent_directories(const std::filesystem::path &source);
    virtual void effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest);

private:
    std::shared_ptr<IConfig> m_config;
};

#endif