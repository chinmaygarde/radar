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

  findDefinitions(_document->child("svg"));
}

SVGArchive::~SVGArchive() = default;

bool SVGArchive::isValid() const {
  return _document != nullptr;
}

void SVGArchive::findDefinitions(const pugi::xml_node& node) {
  if (node.empty()) {
    return;
  }

  auto attribute = node.attribute("id");

  if (!attribute.empty()) {
    _definitions[attribute.value()] = node;
  }

  for (const auto& child : node.children()) {
    findDefinitions(child);
  }
}

bool SVGArchive::inflate(interface::Interface& interface,
                         interface::ModelEntity& container) const {
  if (!isValid()) {
    return false;
  }

  pugi::xml_node node = _document->child("svg");

  if (node.empty()) {
    return false;
  }

  container.setFrame(Decode<geom::Rect>(node, "viewBox"));

  visitNodeChildren(node, interface, container);

  return true;
}

void SVGArchive::visitNodeChildren(const pugi::xml_node& node,
                                   interface::Interface& interface,
                                   interface::ModelEntity& parent) const {
  for (const auto& child : node.children()) {
    visitNode(child, interface, parent);
  }
}

interface::ModelEntity::Ref SVGArchive::visitNode(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  if (::strncmp(node.name(), "rect", sizeof("rect")) == 0) {
    return visitRect(node, interface, parent);
  }

  if (::strncmp(node.name(), "ellipse", sizeof("ellipse")) == 0) {
    return visitEllipse(node, interface, parent);
  }

  if (::strncmp(node.name(), "g", sizeof("g")) == 0) {
    return visitG(node, interface, parent);
  }

  if (::strncmp(node.name(), "circle", sizeof("circle")) == 0) {
    return visitCircle(node, interface, parent);
  }

  if (::strncmp(node.name(), "polygon", sizeof("polygon")) == 0) {
    return visitPolygon(node, interface, parent);
  }

  if (::strncmp(node.name(), "line", sizeof("line")) == 0) {
    return visitLine(node, interface, parent);
  }

  if (::strncmp(node.name(), "use", sizeof("use")) == 0) {
    return visitUse(node, interface, parent);
  }

  if (::strncmp(node.name(), "text", sizeof("text")) == 0) {
    return visitText(node, interface, parent);
  }

  if (::strncmp(node.name(), "path", sizeof("path")) == 0) {
    return visitPath(node, interface, parent);
  }

  RL_LOG("Unknown: %s", node.name());
  return nullptr;
}

static void ReadCommonEntityProperties(const pugi::xml_node& node,
                                       interface::ModelEntity& entity) {
  entity.setBackgroundColor(Decode<entity::Color>(node, "fill"));
  entity.setTransformation(Decode<geom::Matrix>(node, "transform"));

  /*
   *  TODO:
   *  - stroke
   *  - stroke-width
   */
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#RectElement
 */
interface::ModelEntity::Ref SVGArchive::visitRect(
    const pugi::xml_node& node,
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
    return nullptr;
  }

  entity->setFrame(frame);

  /*
   *  TODO: Radii are not handled here.
   */

  parent.addChild(entity);

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#EllipseElement
 */
interface::ModelEntity::Ref SVGArchive::visitEllipse(
    const pugi::xml_node& node,
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
    return nullptr;
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

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/struct.html#GElement
 */
interface::ModelEntity::Ref SVGArchive::visitG(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();

  /*
   *  TODO: Implement the rest of this element.
   */

  parent.addChild(entity);

  visitNodeChildren(node, interface, *entity);

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#CircleElement
 */
interface::ModelEntity::Ref SVGArchive::visitCircle(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();

  ReadCommonEntityProperties(node, *entity);

  double radius = Decode<double>(node, "r");

  if (radius <= 0.0) {
    /*
     *  A value of zero disables rendering of the element.
     */
    return nullptr;
  }

  const geom::Point center = {
      Decode<double>(node, "cx"),  //
      Decode<double>(node, "cy"),  //
  };

  geom::PathBuilder builder;

  builder.addCircle(center, radius);

  entity->setPath(builder.path());

  parent.addChild(entity);

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#PolygonElement
 */
interface::ModelEntity::Ref SVGArchive::visitPolygon(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  auto points = Decode<std::vector<geom::Point>>(node, "points");

  if (points.size() == 0) {
    return nullptr;
  }

  auto entity = interface.createEntity();

  ReadCommonEntityProperties(node, *entity);

  geom::PathBuilder builder;

  builder.moveTo(*points.begin());

  for (const auto& point : points) {
    builder.lineTo(point);
  }

  entity->setPath(builder.path());

  parent.addChild(entity);

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#LineElement
 */
interface::ModelEntity::Ref SVGArchive::visitLine(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  auto entity = interface.createEntity();

  ReadCommonEntityProperties(node, *entity);

  geom::PathBuilder builder;

  builder.moveTo({
      Decode<double>(node, "x1"),  //
      Decode<double>(node, "y1"),  //
  });

  builder.lineTo({
      Decode<double>(node, "x2"),  //
      Decode<double>(node, "y2"),  //
  });

  entity->setPath(builder.path());

  parent.addChild(entity);

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/struct.html#UseElement
 */
interface::ModelEntity::Ref SVGArchive::visitUse(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  auto link = Decode<core::URI>(node, "xlink:href");

  if (!link.isValid()) {
    return nullptr;
  }

  auto fragment = link.fragment();

  if (fragment.size() == 0) {
    return nullptr;
  }

  auto found = _definitions.find(fragment);

  if (found == _definitions.end()) {
    return nullptr;
  }

  auto entity = visitNode(found->second, interface, parent);

  if (entity == nullptr) {
    return nullptr;
  }

  /*
   *  The ‘use’ element has optional attributes ‘x’, ‘y’, ‘width’ and ‘height’.
   *  If on visiting this node, we inflated a model entity, attach these
   *  attributes *if present* on the same.
   */

  geom::Rect frame = entity->frame();

  bool present = false;

  auto x = Decode<double>(node, "x", &present);
  if (present) {
    frame.origin.x = x;
  }

  auto y = Decode<double>(node, "y", &present);
  if (present) {
    frame.origin.y = y;
  }

  auto width = Decode<double>(node, "width", &present);
  if (present) {
    frame.size.width = width;
  }

  auto height = Decode<double>(node, "height", &present);
  if (present) {
    frame.size.height = height;
  }

  entity->setFrame(frame);

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG/text.html#TextElement
 */
interface::ModelEntity::Ref SVGArchive::visitText(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  /*
   *  TODO: Wire up text elements when libTypography work items are completed.
   */
  return nullptr;
}

/**
 *  https://www.w3.org/TR/SVG/paths.html#PathElement
 */
interface::ModelEntity::Ref SVGArchive::visitPath(
    const pugi::xml_node& node,
    interface::Interface& interface,
    interface::ModelEntity& parent) const {
  /*
   *  TODO: Wire up path elements when path data parsing is done.
   */
  return nullptr;
}

}  // namespace ib
}  // namespace rl
