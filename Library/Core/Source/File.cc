// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/File.h>
#include <Core/FileHandle.h>
#include <sys/types.h>
#include <unistd.h>

namespace rl {
namespace core {

File::File(const FileHandle& handle) : _valid(false) {
  if (!handle.isValid()) {
    return;
  }

  _valid = RL_TEMP_FAILURE_RETRY(::fstat(handle.handle(), &_stat)) == 0;
}

File::File(const URI& uri) : _valid(false) {
  if (!uri.isValid()) {
    return;
  }

  _valid = RL_TEMP_FAILURE_RETRY(
               ::stat(uri.filesystemRepresentation().c_str(), &_stat)) == 0;
}

File::File(File&& other) = default;

File::~File() = default;

bool File::isValid() const {
  return _valid;
}

File::Type File::type() const {
  if (!_valid) {
    return Type::Unknown;
  }

  if (S_ISREG(_stat.st_mode)) {
    return Type::File;
  }

  if (S_ISDIR(_stat.st_mode)) {
    return Type::Directory;
  }

  if (S_ISCHR(_stat.st_mode)) {
    return Type::CharacterDevice;
  }

  if (S_ISBLK(_stat.st_mode)) {
    return Type::BlockDevice;
  }

  if (S_ISFIFO(_stat.st_mode)) {
    return Type::NamedPipe;
  }

  if (S_ISLNK(_stat.st_mode)) {
    return Type::SymbolicLink;
  }

  if (S_ISSOCK(_stat.st_mode)) {
    return Type::Socket;
  }

  return Type::Unknown;
}

size_t File::size() const {
  return _valid ? _stat.st_size : 0;
}

bool File::SetAsWorkingDirectory(const URI& uri) {
  if (!uri.isValid()) {
    return false;
  }

  return RL_TEMP_FAILURE_RETRY(
             ::chdir(uri.filesystemRepresentation().c_str())) == 0;
}

}  // namespace core
}  // namespace rl
