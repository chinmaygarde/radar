// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>
#include "SVGArchive.h"

namespace rl {
namespace ib {

std::unique_ptr<InterfaceBuilderArchive> InterfaceBuilderArchive::Make(
    const uint8_t* data,
    size_t size) {
  if (data == nullptr || size == 0) {
    return nullptr;
  }

  /*
   *  In fallback order, check for recognized archive formats.
   */
  auto svgArchive = core::make_unique<SVGArchive>(data, size);

  if (svgArchive->isValid()) {
    return std::move(svgArchive);
  }

  /*
   *  Add more formats as they are supported.
   */
  return nullptr;
}

std::unique_ptr<InterfaceBuilderArchive> InterfaceBuilderArchive::Make(
    const core::FileHandle& handle) {
  if (!handle.isValid()) {
    return nullptr;
  }

  core::FileMapping mapping(handle);

  if (mapping.mapping() == nullptr || mapping.size() == 0) {
    return nullptr;
  }

  return Make(mapping.mapping(), mapping.size());
}

std::unique_ptr<InterfaceBuilderArchive> InterfaceBuilderArchive::Make(
    const core::Allocation& allocation) {
  if (allocation.size() == 0) {
    return nullptr;
  }

  return Make(allocation.data(), allocation.size());
}

InterfaceBuilderArchive::InterfaceBuilderArchive() = default;

InterfaceBuilderArchive::~InterfaceBuilderArchive() = default;

}  // namespace ib
}  // namespace rl
