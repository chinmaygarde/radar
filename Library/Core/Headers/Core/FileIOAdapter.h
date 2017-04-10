/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Allocation.h>
#include <Core/FileHandle.h>
#include <Core/Macros.h>
#include <cstddef>

namespace rl {
namespace core {

class FileIOAdapter {
 public:
  enum class Whence {
    Set,
    Current,
    End,
  };

  using Offset = ptrdiff_t;

  FileIOAdapter(FileHandle handle);

  ~FileIOAdapter();

  bool isValid() const;

  RL_WARN_UNUSED_RESULT
  bool seek(Whence whence, Offset offset);

  RL_WARN_UNUSED_RESULT
  size_t write(const uint8_t* buffer, size_t size);

  RL_WARN_UNUSED_RESULT
  size_t write(const Allocation& allocation);

 private:
  FileHandle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(FileIOAdapter);
};

}  // namespace core
}  // namespace rl
