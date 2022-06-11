#include "../src/config/config.hpp"

#include <gtest/gtest.h>

TEST(TestConfig, test_merge_backup_dry_run)
{
    // Arrange
    std::shared_ptr<Config> config = std::make_shared<Config>();
    std::shared_ptr<Config> source = std::make_shared<Config>();

    source->backup.dry_run = true;

    // Act
    config->merge(source);

    // Assert
    ASSERT_TRUE(config->backup.dry_run);
}

TEST(TestConfig, test_merge_backup)
{
    // Arrange
    std::shared_ptr<Config> config = std::make_shared<Config>();
    std::shared_ptr<Config> source = std::make_shared<Config>();
    std::string expected_backup_strategy = "keep-nth";
    int expected_backup_nth{10};
    std::filesystem::path expected_backup_config_file{"/hello/world"};
    std::filesystem::path expected_backup_destination{"/home/destination"};
    std::filesystem::path expected_backup_dir_name{"/where/is/my/mind"};
    CommandLineType expected_action{CommandLineType::Backup};
    std::string expected_path_str{"hello"};
    std::set<std::string> expected_exclusion_paths{expected_path_str};
    std::string expected_regex_str{"my-regex"};
    std::vector<std::regex> expected_regex{std::regex{expected_regex_str}};
    std::string expected_directory_str{"hello/world"};
    std::string expected_root_path("/h/e/l/o/world");

    source->action = expected_action;
    source->backup.verbose = true;
    source->backup.strategy = expected_backup_strategy;
    source->backup.nth = expected_backup_nth;
    source->backup.config_file = expected_backup_config_file;
    source->backup.destination = expected_backup_destination;
    source->backup.backup_dir_name = expected_backup_dir_name;
    source->exclusion_paths.insert(expected_path_str);
    source->exclusion_patterns = expected_regex;

    source->include_directories.insert("/Hello");
    source->include_directories.insert("/World");

    source->root_path = expected_root_path;

    // Act
    config->merge(source);

    // Assert
    ASSERT_EQ(config->action, expected_action);
    ASSERT_FALSE(config->backup.dry_run);
    ASSERT_TRUE(config->backup.verbose);
    ASSERT_EQ(config->backup.strategy, expected_backup_strategy);
    ASSERT_EQ(config->backup.nth, expected_backup_nth);
    ASSERT_EQ(config->backup.config_file, expected_backup_config_file);
    ASSERT_EQ(config->backup.destination, expected_backup_destination);
    ASSERT_EQ(config->backup.backup_dir_name, expected_backup_dir_name);
    ASSERT_EQ(config->exclusion_paths, expected_exclusion_paths);
    ASSERT_FALSE(config->exclusion_patterns.empty());
    ASSERT_FALSE(config->include_directories.empty());
    ASSERT_EQ(config->root_path, expected_root_path);
}

