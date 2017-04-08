/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/TempFileHandle.h>
#include <TestRunner/TestRunner.h>

TEST(TempFileTest, TempFileCreate) {
  auto handle = rl::core::TemporaryFileCreate();
  ASSERT_TRUE(handle.isValid());
}

TEST(TempFileTest, TempDirectoryGet) {
  auto uri = rl::core::TemporaryFileDirectory();
  ASSERT_TRUE(uri.isValid());
}
