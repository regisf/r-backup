#ifndef FILE_COPY_HPP
#define FILE_COPY_HPP

#include "../config/config.hpp"
#include "enums.hpp"

#include <filesystem>
#include <memory>
#include <utility>

namespace utils
{
    class IFileCopy
    {
    public:
        virtual ~IFileCopy() = default;
        virtual void copy_file(const std::filesystem::path &source, const std::filesystem::path &dest) = 0;
        virtual void effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest) = 0;
    };

    /**
     * @brief FileCopy implementation
     */
    class FileCopy : public IFileCopy
    {
    public:
        explicit FileCopy() = default;
        ~FileCopy() override = default;

        void copy_file(const std::filesystem::path &source, const std::filesystem::path &dest) override;
        void effective_copy(const std::filesystem::path &source, const std::filesystem::path &dest) override;
    };
}
#endif
