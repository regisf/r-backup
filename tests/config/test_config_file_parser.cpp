
#include "../../src/config/config_file_parser.hpp"
#include "../mock/exit.hpp"

#include <gtest/gtest.h>

TEST(TestConfigFileParser, test_to_config)
{
    // Arrange (/ are automatically added at the end of a path
    std::filesystem::path expected_root = std::filesystem::current_path() / "mixins/source/";
    std::filesystem::path expected_name = std::filesystem::current_path() / "mixins/dest/r-backup";
    std::vector<std::regex> expected_excluded_patterns{std::regex{"^exclude"}};
    std::vector<std::string> expected_excluded_paths{
            std::string{std::filesystem::current_path() / "mixins/source/truc/.ignore"}};
    std::set<std::string> expected_included_paths{std::filesystem::current_path() / "mixins/source/one",
                                                  std::filesystem::current_path() / "mixins/source/truc"};

    mock::exit_setup();

    ConfigFileParser parser;
    char *root_path = std::getenv("CONFIG_ROOT_PATH");
    std::string base_path = root_path != nullptr ? std::string{root_path} : std::string{"../.."};
    parser.parse_file(base_path + "/mixins/config.yaml", mock::exit);

    // Act
    auto config = parser.to_config();

    // Assert
    ASSERT_FALSE(mock::is_exit_called());
    ASSERT_EQ(config->root_path, expected_root);
    ASSERT_EQ(config->backup.destination, expected_name);
    ASSERT_TRUE(std::equal(config->exclusion_paths.begin(),
                           config->exclusion_paths.end(),
                           expected_excluded_paths.begin()));
    // Find a way to compare regexes
    ASSERT_TRUE(std::equal(config->include_directories.begin(),
                           config->include_directories.end(),
                           expected_included_paths.begin()));

    // Restore
    mock::exit_tear_down();
}
