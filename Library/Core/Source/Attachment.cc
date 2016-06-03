// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Attachment.h>

namespace rl {
namespace core {

Attachment::Attachment() = default;

Attachment::~Attachment() = default;

Attachment::Attachment(Attachment::Handle handle)
    : _handle(std::make_shared<Attachment::Handle>(handle)) {}

bool Attachment::isValid() const {
  return _handle != nullptr;
}

Attachment::Handle Attachment::handle() const {
  return _handle != nullptr ? *_handle : -1;
}

void Attachment::invalidate() {
  _handle = nullptr;
}

}  // namespace core
}  // namespace rl
