// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_FILEMAPPING_H_
#define RADAR_CORE_FILEMAPPING_H_

#include <Core/Macros.h>
#include <Core/FileHandle.h>

namespace rl {
namespace core {

class FileMapping {
 public:
  FileMapping();

  FileMapping(const FileHandle& handle, size_t size);

  FileMapping(FileMapping&& mapping);

  ~FileMapping();

  uint8_t* mapping() const;

  size_t size() const;

 private:
  uint8_t* _mapping;
  size_t _size;

  RL_DISALLOW_COPY_AND_ASSIGN(FileMapping);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_FILEMAPPING_H_