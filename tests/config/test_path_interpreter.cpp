#include "../../src/config/path_interpreter.hpp"
#include "../../src/exceptions/path_interpreter_exception.hpp"

#include <filesystem>

#include <gtest/gtest.h>

TEST(TestPathInterpreter, test_interpret_nothing)
{
    // Arrange
    std::filesystem::path path{"/hello/world"};
    PathInterpreter interpreter{path};

    // Act
    auto response = interpreter.normalize_path();

    // Assert
    ASSERT_EQ(path, response);
}

TEST(TestPathInterpreter, test_interpret_home)
{
    // Arrange
    std::string target_dir{"world"};
    auto home = std::filesystem::path{std::getenv("HOME")} / target_dir;

    std::filesystem::path path{"$HOME/world"};
    PathInterpreter interpreter{path};

    // Act
    auto result = interpreter.normalize_path();

    // Assert
    ASSERT_EQ(home, result);
}

TEST(TestPathInterpreter, test_interpret_pwd)
{
    // Arrange
    std::string target_dir{"world"};
    auto home = std::filesystem::path{std::filesystem::current_path() / target_dir};

    std::filesystem::path path{"$PWD/world"};
    PathInterpreter interpreter{path};

    // Act
    auto result = interpreter.normalize_path();

    // Assert
    ASSERT_EQ(home, result);
}

TEST(TestPathInterpreter, test_interpret_with_home_not_at_the_begining_should_throw_an_error)
{
    // Arrange
    std::filesystem::path path{"/where/the/home/is/$HOME/world"};
    PathInterpreter interpreter{path};

    // Act
    try
    {
        auto result = interpreter.normalize_path();
        FAIL();
    }
    catch (PathInterpreterException &e)
    {
        SUCCEED();
    }
}
