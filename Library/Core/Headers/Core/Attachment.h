// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <functional>
#include <memory>

namespace rl {
namespace core {

class Attachment {
 public:
  using Handle = intptr_t;

  class MessageHandle {
   public:
    using Collector = std::function<void(Handle handle)>;

    MessageHandle(Handle handle);

    MessageHandle(Handle handle, Collector collector);

    MessageHandle(MessageHandle&& other);

    ~MessageHandle();

    Handle handle() const;

   private:
    Handle _handle;
    Collector _collector;

    RL_DISALLOW_COPY_AND_ASSIGN(MessageHandle);
  };

  virtual Handle handle() const = 0;

  virtual MessageHandle messageHandle() const;
};

using AttachmentRef = std::shared_ptr<Attachment>;

}  // namespace core
}  // namespace rl
