// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGArchive.h"

namespace rl {
namespace ib {

SVGArchive::SVGArchive(const core::FileHandle& handle) {
  core::FileMapping mapping(handle);

  if (mapping.size() == 0) {
    return;
  }

  auto document = core::make_unique<pugi::xml_document>();

  auto result = document->load_buffer(mapping.mapping(), mapping.size());

  if (result.status != pugi::xml_parse_status::status_ok) {
    return;
  }

  _document = std::move(document);
}

SVGArchive::~SVGArchive() = default;

bool SVGArchive::isValid() const {
  return _document != nullptr;
}

bool SVGArchive::inflate(interface::Interface& interface) const {
  if (!isValid()) {
    return false;
  }

  pugi::xml_node svg = _document->child("svg");

  if (svg.empty()) {
    return false;
  }

  auto parent = interface.createEntity();

  for (const auto& child : svg.children()) {
    if (::strncmp(child.name(), "rect", sizeof("rect")) == 0) {
      visitRect(child, interface, parent);
      continue;
    }
  }

  return true;
}

void SVGArchive::visitRect(const pugi::xml_node& node,
                           interface::Interface& interface,
                           interface::ModelEntity& parent) const {
  /*
   *  WIP
   */
}

}  // namespace ib
}  // namespace rl
