// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGArchive.h"
#include "SVGDecoder.h"
#include <Geometry/PathBuilder.h>

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

    if (::strncmp(child.name(), "ellipse", sizeof("ellipse")) == 0) {
      visitEllipse(child, interface, container);
      continue;
    }

    if (::strncmp(child.name(), "g", sizeof("g")) == 0) {
      visitG(child, interface, container);
      continue;
    }

    if (::strncmp(child.name(), "circle", sizeof("circle")) == 0) {
      visitCircle(child, interface, container);
      continue;
    }
  }

  return true;
}

static void ReadCommonEntityProperties(const pugi::xml_node& node,
                                       interface::ModelEntity& entity) {
  entity.setBackgroundColor(Decode<entity::Color>(node, "fill"));
  entity.setTransformation(Decode<geom::Matrix>(node, "transform"));
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#RectElement
 */
void SVGArchive::visitRect(const pugi::xml_node& node,
                           interface::Interface& interface,
                           interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();

  ReadCommonEntityProperties(node, *entity);

  const geom::Rect frame = {
      Decode<double>(node, "x"),       //
      Decode<double>(node, "y"),       //
      Decode<double>(node, "width"),   //
      Decode<double>(node, "height"),  //
  };

  if (frame.size.width <= 0.0 || frame.size.height <= 0.0) {
    /*
     *  A value of zero disables rendering of the element.
     */
    return;
  }

  entity->setFrame(frame);

  /*
   *  TODO: Radii are not handled here.
   */

  parent.addChild(entity);
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#EllipseElement
 */
void SVGArchive::visitEllipse(const pugi::xml_node& node,
                              interface::Interface& interface,
                              interface::ModelEntity& parent) const {
  const geom::Size size = {
      Decode<double>(node, "rx") * 2.0,  //
      Decode<double>(node, "ry") * 2.0,  //
  };

  if (size.width <= 0.0 && size.height <= 0.0) {
    /*
     *  A value of zero disables rendering of the element.
     */
    return;
  }

  const geom::Point center = {
      Decode<double>(node, "cx"),  //
      Decode<double>(node, "cy"),  //
  };

  geom::PathBuilder builder;

  builder.addEllipse(center, size);

  auto entity = interface.createEntity();

  ReadCommonEntityProperties(node, *entity);

  entity->setPath(builder.path());

  parent.addChild(entity);
}

/*
 *  https://www.w3.org/TR/SVG11/struct.html#GElement
 */
void SVGArchive::visitG(const pugi::xml_node& node,
                        interface::Interface& interface,
                        interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();

  /*
   *  TODO: Implement the rest of this element.
   */

  parent.addChild(entity);
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#CircleElement
 */
void SVGArchive::visitCircle(const pugi::xml_node& node,
                             interface::Interface& interface,
                             interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();

  ReadCommonEntityProperties(node, *entity);

  double radius = Decode<double>(node, "r");

  if (radius <= 0.0) {
    /*
     *  A value of zero disables rendering of the element.
     */
    return;
  }

  const geom::Point center = {
      Decode<double>(node, "cx"),  //
      Decode<double>(node, "cy"),  //
  };

  geom::PathBuilder builder;

  builder.addCircle(center, radius);

  entity->setPath(builder.path());

  parent.addChild(entity);
}

}  // namespace ib
}  // namespace rl
