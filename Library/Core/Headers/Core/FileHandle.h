// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_FILEHANDLE_H_
#define RADAR_CORE_FILEHANDLE_H_

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Core/RawAttachment.h>

namespace rl {
namespace core {

class MachFilePort;

class FileHandle : public Attachment {
 public:
  using Handle = int;

  FileHandle(RawAttachment attachment);

  FileHandle(const URI& uri);

  FileHandle(FileHandle&& file);

  ~FileHandle();

  bool isValid() const override;

  Attachment::Handle handle() const override;

  RL_WARN_UNUSED_RESULT
  Attachment::Handle takeHandle() override;

 private:
  Handle _handle;

#if RL_CHANNELS == RL_CHANNELS_MACH
  mutable std::unique_ptr<MachFilePort> _port;
#endif

  RL_DISALLOW_COPY_AND_ASSIGN(FileHandle);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_FILEHANDLE_H_
