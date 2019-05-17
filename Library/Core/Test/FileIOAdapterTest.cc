/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/FileIOAdapter.h>
#include <Core/TempFileHandle.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(FileIOAdapterTest, SimpleWrite) {
  FileIOAdapter adapter(TemporaryFileCreate(false));

  ASSERT_TRUE(adapter.seek(FileIOAdapter::Whence::Set, 0));

  const uint8_t data[] = "Hello";
  ASSERT_EQ(adapter.write(data, sizeof(data)), sizeof(data));
}

}  // namespace testing
}  // namespace core
}  // namespace rl
