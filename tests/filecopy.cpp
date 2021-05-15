#include "fakeit/single_header/gtest"
#include <gtest/gtest.h>

TEST(test_filecopy, copy_file)
{
    // Assert
    Mock<IConfig> mockConfig;
    When(Method(mockConfig, is_destination_dir_exists)).Return(true);

    FileCopy fileCopy{mockConfig};


    // Act

    // Arrange
}