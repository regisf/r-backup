#include "../../src/config.hpp"
#include "../../src/utils/filecopy.hpp"

#include <fakeit.hpp>
#include <gtest/gtest.h>


using namespace fakeit;

TEST(test_filecopy, copy_file)
{
    // Assert
    std::shared_ptr<Config> config = std::make_shared<Config>();
    Mock<IConfig> mockConfig{*config};

    When(Method(mockConfig, is_destination_dir_exists)).Return(true);

    FileCopy fileCopy{config};

    // Act

    // Arrange
}