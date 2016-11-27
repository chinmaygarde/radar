// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGArchive.h"
#include "SVGDecoder.h"

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

bool SVGArchive::inflate(interface::Interface& interface,
                         interface::ModelEntity& container) const {
  if (!isValid()) {
    return false;
  }

  pugi::xml_node svg = _document->child("svg");

  if (svg.empty()) {
    return false;
  }

  container.setFrame(Decode<geom::Rect>(svg, "viewBox"));

  for (const auto& child : svg.children()) {
    if (::strncmp(child.name(), "rect", sizeof("rect")) == 0) {
      visitRect(child, interface, container);
      continue;
    }
  }

  return true;
}

void SVGArchive::visitRect(const pugi::xml_node& node,
                           interface::Interface& interface,
                           interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();
  parent.addChild(entity);

  entity.setBounds(Decode<geom::Rect>(node));
  entity.setBackgroundColor(Decode<entity::Color>(node, "fill"));
}

}  // namespace ib
}  // namespace rl
