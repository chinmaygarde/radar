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

  Attachment();

  ~Attachment();

  Attachment(Handle handle);

  bool isValid() const;

  Handle handle() const;

  void invalidate();

 private:
  std::shared_ptr<Handle> _handle;
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_ATTACHMENT_H_
