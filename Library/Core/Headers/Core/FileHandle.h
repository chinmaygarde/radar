// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <Core/RawAttachment.h>
#include <Core/URI.h>

namespace rl {
namespace core {

class FileHandle : public Attachment {
 public:
  enum class AccessType {
    Read,
    Write,
    ReadWrite,
  };

  FileHandle();

  FileHandle(Handle handle);

  FileHandle(RawAttachment attachment);

  FileHandle(const URI& uri);

  FileHandle(const URI& uri, AccessType type, bool createIfNecessary);

  FileHandle(FileHandle&& file);

  ~FileHandle();

  bool isValid() const;

  Handle handle() const override;

  MessageHandle messageHandle() const override;

 private:
  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(FileHandle);
};

}  // namespace core
}  // namespace rl
