// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_FILEHANDLE_H_
#define RADAR_CORE_FILEHANDLE_H_

#include <Core/Macros.h>
#include <Core/URI.h>

namespace rl {
namespace core {

class FileHandle {
 public:
  using Handle = int;

  FileHandle(const URI& uri);

  FileHandle(FileHandle&& file);

  ~FileHandle();

  bool isValid() const;

  Handle handle() const;

 private:
  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(FileHandle);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_FILEHANDLE_H_
