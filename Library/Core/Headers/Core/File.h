// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_FILE_H_
#define RADAR_CORE_FILE_H_

#include <Core/Macros.h>
#include <Core/URI.h>

namespace rl {
namespace core {

class File {
 public:
  using Handle = int;

  File(const URI& uri);

  File(File&& file);

  ~File();

  bool isReady() const;

 private:
  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(File);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_FILE_H_
