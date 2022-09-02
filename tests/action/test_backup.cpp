#include "../../src/action/backup.hpp"
#include "../mock/exit.hpp"

#include <gtest/gtest.h>


TEST(TestBackup, test_backup_start_and_cant_backup)
{
    // Arrange
    mock::exit_setup();

    // Act
    action::Backup::start(mock::exit);

    // Assert
    ASSERT_TRUE(mock::is_exit_called());
    mock::exit_tear_down();
}
