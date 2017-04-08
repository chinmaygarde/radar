/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/FileHandle.h>
#include <Core/Macros.h>

namespace rl {
namespace core {

class FileMapping {
 public:
  FileMapping();

  FileMapping(const FileHandle& handle);

  FileMapping(const FileHandle& handle, size_t size);

  FileMapping(FileMapping&& mapping);

  ~FileMapping();

  uint8_t* mapping() const;

  size_t size() const;

 private:
  uint8_t* _mapping;
  size_t _size;

  void setupMapping(const FileHandle& handle, size_t size);

  RL_DISALLOW_COPY_AND_ASSIGN(FileMapping);
};

}  // namespace core
}  // namespace rl
