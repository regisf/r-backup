#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "../../src/action/path_explorer.hpp"
#include "../mock/sym_link.hpp"

#include <gtest/gtest.h>


TEST(TestPathExplorer, test_pattern_should_be_skipped)
{
    // Arrange
    std::shared_ptr<Config> config = Config::instance();
    std::regex regex{"\\.git"};
    config->exclusion_patterns.push_back(regex);

    PathExplorer pathExplorer;
    std::filesystem::path path{".git"};

    // Act
    auto result = pathExplorer.should_be_skipped(path);

    // Assert
    ASSERT_TRUE(result);
}

TEST(TestPathExplorer, test_pattern_should_not_be_skipped)
{
    // Arrange
    std::shared_ptr<Config> config = Config::instance();
    std::regex regex{"\\.git"};
    config->exclusion_patterns.push_back(regex);

    PathExplorer pathExplorer;
    std::filesystem::path path{".got"};

    // Act
    auto result = pathExplorer.should_be_skipped(path);

    // Assert
    ASSERT_FALSE(result);
}

TEST(TestPathExplorer, test_exclude_dir_should_be_skipped)
{
    // Arrange
    std::shared_ptr<Config> config = Config::instance();
    std::filesystem::path exclude_path{"toto_titi"};
    config->exclusion_paths.insert("toto_titi");
    config->root_path = "/home/test";

    PathExplorer pathExplorer;

    // Act
    auto result = pathExplorer.should_be_skipped(exclude_path);

    // Assert
    ASSERT_TRUE(result);
}

TEST(TestPathExplorer, test_exclude_dir_should_not_be_skipped)
{
    // Arrange
    std::shared_ptr<Config> config = Config::instance(true);
    std::filesystem::path exclude_path{"toto_titi"};
    config->exclusion_paths.insert("tata");
    config->root_path = "/home/test";

    PathExplorer pathExplorer;

    // Act
    auto result = pathExplorer.should_be_skipped(exclude_path);

    // Assert
    ASSERT_FALSE(result);
}

TEST(TestPathExplorer, test_symlink_returns_true)
{
    // Arrange
    std::shared_ptr<Config> config = Config::instance();
    std::filesystem::path exclude_path{"toto_titi"};
    config->exclusion_paths.insert("tata");

    PathExplorer pathExplorer;

    // Act
    auto result = pathExplorer.should_be_skipped(exclude_path,
                                                 mock::is_sym_link_always_true);

    // Assert
    ASSERT_TRUE(result);
}

TEST(TestPathExplorer, test_symlink_returns_false)
{
    // Arrange
    std::shared_ptr<Config> config = Config::instance();
    std::filesystem::path exclude_path{"toto_titi"};
    config->exclusion_paths.insert("tata");

    PathExplorer pathExplorer;

    // Act
    auto result = pathExplorer.should_be_skipped(exclude_path,
                                                 mock::is_symlink_always_false);

    // Assert
    ASSERT_FALSE(result);
}

#pragma clang diagnostic pop
