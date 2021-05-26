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

/**
 * @brief FileCopy implementation
 */
class FileCopy : public IFileCopy
{
public:
    FileCopy() = default;
    FileCopy(std::shared_ptr<Config> config) : m_config(config) {}
    
    virtual void copy_file(const std::filesystem::path &source, const std::filesystem::path &dest);
    virtual Status create_parent_directories(const std::filesystem::path &source);
    virtual void effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest);
    virtual void set_config(std::shared_ptr<Config> config);

private:
    std::shared_ptr<Config> m_config;
};

#endif
