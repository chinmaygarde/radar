/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/TempFileHandle.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(TempFileTest, TempFileCreate) {
  auto handle = TemporaryFileCreate();
  ASSERT_TRUE(handle.isValid());
}

TEST(TempFileTest, TempDirectoryGet) {
  auto uri = TemporaryFileDirectory();
  ASSERT_TRUE(uri.isValid());
}

}  // namespace testing
}  // namespace core
}  // namespace rl
