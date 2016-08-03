// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_SOCKETPAIR_H_
#define RADAR_CORE_SOCKETPAIR_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>
#include <Core/Attachment.h>
#include <Core/RawAttachment.h>

namespace rl {
namespace core {

class SocketPair : public Attachment {
 public:
  using Handle = int;

  static const Handle kInvalidHandle;
  static const size_t kDefaultSocketBufferSize;

  SocketPair(size_t bufferSize = kDefaultSocketBufferSize);

  SocketPair(RawAttachment attachment,
             size_t bufferSize = kDefaultSocketBufferSize);

  SocketPair(SocketPair&&);

  ~SocketPair();

  Handle readHandle() const;

  Handle writeHandle() const;

  bool isValid() const;

  Attachment::Handle handle() const override;

  static bool ConfigureSocketHandle(SocketPair::Handle handle,
                                    size_t bufferSize,
                                    bool blocking);

 private:
  Handle _readHandle;
  Handle _writeHandle;
  bool _isValid;

  RL_DISALLOW_COPY_AND_ASSIGN(SocketPair);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADAR_CORE_SOCKETPAIR_H_
