// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGArchive.h"

namespace rl {
namespace ib {

SVGArchive::SVGArchive(core::FileHandle handle)
    : _handle(std::move(handle)), _isValid(false) {
  if (!_handle.isValid()) {
    return;
  }
}

SVGArchive::~SVGArchive() = default;

bool SVGArchive::isArchiveReadable() const {
  return _isValid;
}

std::unique_ptr<entity::Entity> SVGArchive::onInflate() const {
  return nullptr;
}

}  // namespace ib
}  // namespace rl
