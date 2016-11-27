// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>
#include "SVGArchive.h"

namespace rl {
namespace ib {

std::unique_ptr<InterfaceBuilderArchive> InterfaceBuilderArchive::Make(
    const core::FileHandle& handle) {
  if (!handle.isValid()) {
    return nullptr;
  }

  /*
   *  In fallback order, check for recognized archive formats.
   */
  auto svgArchive = core::make_unique<SVGArchive>(handle);

  if (svgArchive->isValid()) {
    return std::move(svgArchive);
  }

  /*
   *  Add more formats as they are supported.
   */
  return nullptr;
}

InterfaceBuilderArchive::InterfaceBuilderArchive() = default;

InterfaceBuilderArchive::~InterfaceBuilderArchive() = default;

bool InterfaceBuilderArchive::isValid() const {
  return _impl ? _impl->isArchiveReadable() : false;
}

std::unique_ptr<entity::Entity> InterfaceBuilderArchive::inflate() const {
  return isValid() ? _impl->onInflate() : nullptr;
}

}  // namespace ib
}  // namespace rl
