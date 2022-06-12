#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "../../src/config/config_file_parser.hpp"

#include <gtest/gtest.h>


TEST(TestConfigFileParser, test_to_config)
{
    // Arrange (/ are automatically added at the end of a path
    std::filesystem::path expected_root{"mixins/source/"};
    std::filesystem::path expected_name{"mixins/dest/r-backup"};
    std::vector<std::regex> expected_excluded_patterns{std::regex{"^exclude"}};
    std::vector<std::string> expected_excluded_paths{std::string{"mixins/source/truc"}};
    std::set<std::string> expected_included_paths{"mixins/source/one"};

    ConfigFileParser parser;
    parser.parse_file("../../mixins/config.yaml");

    // Act
    auto config = parser.to_config();

    // Assert
    ASSERT_EQ(config->root_path, expected_root);
    ASSERT_EQ(config->backup.destination, expected_name);
    ASSERT_TRUE(std::equal(config->exclusion_paths.begin(),
                           config->exclusion_paths.end(),
                           expected_excluded_paths.begin()));
    // Find a way to compare regexes
    ASSERT_TRUE(std::equal(config->include_directories.begin(),
                           config->include_directories.end(),
                           expected_included_paths.begin()));
}

#pragma clang diagnostic pop
