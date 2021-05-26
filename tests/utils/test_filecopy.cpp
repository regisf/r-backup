#include "../../src/config.hpp"
#include "../../src/utils/filecopy.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/utils/filecopy.hpp"


class MockConfig : public Config
{
public:
    MOCK_METHOD(std::filesystem::path, get_destination_directory, ());
    MOCK_METHOD(const bool, backup_exists, ());
    MOCK_METHOD(std::filesystem::path, get_, ());
    MOCK_METHOD(std::filesystem::path, get_destination_directory, (const std::filesystem::path &path));
    MOCK_METHOD(const bool, is_destination_dir_exists, (const std::filesystem::path &source));
};


class MockFileCopy : public FileCopy
{
public:
    MOCK_METHOD(void, copy_file, (const std::filesystem::path &source, const std::filesystem::path &dest));
    MOCK_METHOD(Status, create_parent_directory, (const std::filesystem::path& source));
    MOCK_METHOD(void ,effective_copy, (const std::filesystem::path &source, const std::filesystem::path &dest));
};


using ::testing::AtLeast;
using ::testing::Return;

TEST(test_filecopy, copy_file_destination_directory_exists)
{
    // Arrange
    std::filesystem::path source{"/a/source/file"};
    std::filesystem::path dest{"/another/destination"};

    std::shared_ptr<MockConfig> mock_config = std::make_shared<MockConfig>();
    ON_CALL(*mock_config, is_destination_dir_exists(source))
        .WillByDefault(Return(true));

    MockFileCopy file_copy;
    EXPECT_CALL(file_copy, effective_copy(source, dest));

    file_copy.set_config(mock_config);

    // Act + Assert
    file_copy.copy_file(source, dest);

}

TEST(test_filecopy, copy_file_destination_directory_doesnt_exist)
{
    ASSERT_TRUE(false);
}
