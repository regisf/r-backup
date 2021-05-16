#include "../../src/config.hpp"
#include "../../src/utils/filecopy.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <fakeit.hpp>

using namespace fakeit;

TEST(test_filecopy, copy_file)
{
    GTEST_SKIP();

    // Arrange
    std::filesystem::path source{"/a/source/file"};
    std::filesystem::path dest{"/another/destination"};

    Mock<IConfig> mock_config;
    Fake(Dtor(mock_config));
    std::shared_ptr<IConfig> config(&mock_config.get());

    When(Method(mock_config, is_destination_dir_exists)).Return(true);
    When(ConstOverloadedMethod(mock_config, get_destination_directory, std::filesystem::path(const std::filesystem::path &))).Return(dest / source);

    FileCopy mock_file_copy{config};
    Mock<FileCopy> spy(mock_file_copy);

    When(OverloadedMethod(spy, create_parent_directories, Status(const std::filesystem::path &)));
    When(Method(spy, effective_copy));

    FileCopy &file_copy = spy.get();

    // Act
    file_copy.copy_file(source, dest);

    // Assert
    VerifyNoOtherInvocations(OverloadedMethod(spy, create_parent_directories, Status(const std::filesystem::path &)));
    Verify(Method(spy, effective_copy)).Exactly(1_Time);
}
