// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_ATTACHMENT_H_
#define RADAR_CORE_ATTACHMENT_H_

#include <Core/Macros.h>

#include <memory>

namespace rl {
namespace core {

class Attachment {
 public:
  using Handle = intptr_t;

  virtual bool isValid() const = 0;

  virtual Handle handle() const = 0;
};

using AttachmentRef = std::shared_ptr<Attachment>;

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_ATTACHMENT_H_
