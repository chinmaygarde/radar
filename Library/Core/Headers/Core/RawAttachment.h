// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Attachment.h>
#include <Core/Macros.h>
#include <functional>

namespace rl {
namespace core {

class RawAttachment : public Attachment {
 public:
  using Collector = std::function<void(Attachment::Handle handle)>;

  RawAttachment();

  RawAttachment(Handle handle, Collector collector);

  RawAttachment(RawAttachment&& other);

  RawAttachment& operator=(RawAttachment&& other);

  ~RawAttachment();

  Handle handle() const override;

  RL_WARN_UNUSED_RESULT
  Handle takeHandle();

 private:
  Attachment::Handle _handle;
  Collector _collector;

  RL_DISALLOW_COPY_AND_ASSIGN(RawAttachment);
};

}  // namespace core
}  // namespace rl
