// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_FILE_H_
#define RADAR_CORE_FILE_H_

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Core/FileHandle.h>

#include <sys/stat.h>

namespace rl {
namespace core {

class File {
 public:
  enum class Type {
    Unknown,
    File,
    Directory,
    CharacterDevice,
    BlockDevice,
    NamedPipe,
    SymbolicLink,
    Socket,
  };

  File(URI uri);

  File(File&& file);

  ~File();

  Type type() const;

  bool isValid() const;

  bool setAsWorkingDirectory() const;

 private:
  struct stat _stat;
  URI _uri;
  bool _valid;

  RL_DISALLOW_COPY_AND_ASSIGN(File);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_FILE_H_
