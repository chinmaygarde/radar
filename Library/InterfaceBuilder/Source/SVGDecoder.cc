// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGDecoder.h"
#include "SVGColorParser/SVGColor.h"
#include <Core/Base64.h>
#include <Entity/Entity.h>
#include <sstream>

namespace rl {
namespace ib {

double DecodeUnit(const char* string) {
  /*
   *  TODO: This needs to handle units. We are just using this method to funnel
   *  all unit decoding so it can be replaced with the correct implementation in
   *  a subsequent pass.
   */
  return atof(string);
}

template <>
double Decode<>(const pugi::xml_node& node, const char* name, bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  return DecodeUnit(attribute.value());
}

template <>
std::string Decode<>(const pugi::xml_node& node,
                     const char* name,
                     bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  return attribute.value();
}

template <>
geom::Rect Decode<>(const pugi::xml_node& node,
                    const char* name,
                    bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  if (attribute.empty()) {
    return {};
  }

  double components[] = {0.0, 0.0, 0.0, 0.0};

  std::istringstream stream;
  stream.str(attribute.value());

  std::string component;
  size_t i = 0;
  for (; (i < 4) && (stream >> component); i++) {
    components[i] = DecodeUnit(component.c_str());
  }

  if (i != 4) {
    return {};
  }

  return {components};
}

template <>
entity::Color Decode<>(const pugi::xml_node& node,
                       const char* name,
                       bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  if (attribute.empty()) {
    return entity::ColorBlackTransparent;
  }

  return ColorFromString(attribute.value());
}

template <>
geom::Matrix Decode<>(const pugi::xml_node& node,
                      const char* name,
                      bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  if (attribute.empty()) {
    return {};
  }

  /*
   *  TODO: Implement the spec!
   *  https://www.w3.org/TR/SVG11/coords.html#TransformAttribute
   */
  return {};
}

template <>
core::URI Decode<>(const pugi::xml_node& node,
                   const char* name,
                   bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  if (attribute.empty()) {
    return {};
  }

  return {std::string{attribute.value()}};
}

template <>
image::Image Decode<>(const pugi::xml_node& node,
                      const char* name,
                      bool* present) {
  auto attribute = node.attribute(name);

  if (present != nullptr) {
    *present = !attribute.empty();
  }

  if (attribute.empty()) {
    return {};
  }

  /*
   *  For now, we only handle base64 encoded data URIs. This is not perfect.
   *  For instance, newlines and whitespaces in the data are not handled.
   */
  const char* base64Marker = ";base64,";

  auto found = ::strstr(attribute.value(), base64Marker);

  if (found == nullptr) {
    return {};
  }

  auto allocation = core::Base64Decode(
      reinterpret_cast<const uint8_t*>(found + strlen(base64Marker)));

  if (allocation.size() == 0) {
    return {};
  }

  return {std::move(allocation)};
}

}  // namespace ib
}  // namespace rl
