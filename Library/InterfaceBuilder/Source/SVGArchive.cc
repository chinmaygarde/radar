// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGArchive.h"
#include <pugixml.hpp>

namespace rl {
namespace ib {

SVGArchive::SVGArchive(const core::FileHandle& handle)
    : _mapping(handle), _isValid(false) {
  if (_mapping.size() == 0) {
    return;
  }

  pugi::xml_document document;
  auto result = document.load_buffer(_mapping.mapping(), _mapping.size());

  if (result.status != pugi::xml_parse_status::status_ok) {
    return;
  }

  _isValid = true;
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
