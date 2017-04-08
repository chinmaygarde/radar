/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/FileIOAdapter.h>
#include <Core/TempFileHandle.h>
#include <TestRunner/TestRunner.h>

TEST(FileIOAdapterTest, SimpleWrite) {
  rl::core::FileIOAdapter adapter(rl::core::TemporaryFileCreate(false));

  ASSERT_TRUE(adapter.seek(rl::core::FileIOAdapter::Whence::Set, 0));

  const uint8_t data[] = "Hello";
  ASSERT_EQ(adapter.write(data, sizeof(data)), sizeof(data));
}
