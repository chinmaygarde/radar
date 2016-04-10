// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/File.h>

#include <fcntl.h>
#include <unistd.h>

#include <string>

namespace rl {
namespace core {

File::File(const std::string& path) : _handle(-1) {
  _handle = RL_TEMP_FAILURE_RETRY(::open(path.c_str(), O_RDONLY));
}

File::~File() {
  if (_handle != -1) {
    RL_TEMP_FAILURE_RETRY_AND_CHECK(::close(_handle));
    _handle = -1;
  }
}

bool File::isReady() const {
  return _handle != -1;
}

}  // namespace core
}  // namespace rl
