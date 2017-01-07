// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGArchive.h"
#include "SVGPathParser/SVGPathString.h"
#include "SVGDecoder.h"
#include <Geometry/PathBuilder.h>
#include <sstream>

namespace rl {
namespace ib {

SVGArchive::SVGArchive(const uint8_t* data, size_t size) {
  if (data == nullptr || size == 0) {
    return;
  }

  auto document = core::make_unique<pugi::xml_document>();

  auto result = document->load_buffer(data, size);

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

static interface::ModelEntity::Ref CreateRegisteredEntity(
    interface::Interface& interface,
    SVGArchive::EntityMap& map,
    const pugi::xml_node& node) {
  auto entity = interface.createEntity();

  auto attribute = node.attribute("id");

  if (attribute.empty()) {
    return entity;
  }

  map[attribute.value()] = entity;
  return entity;
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

interface::ModelEntity::Ref SVGArchive::inflate(interface::Interface& interface,
                                                EntityMap& map) const {
  if (!isValid()) {
    return nullptr;
  }
  return visitNodeChildren(_document->child("svg"), interface, map);
}

static void FixupTransformation(interface::ModelEntity& entity) {
  geom::Matrix identity;
  if (entity.transformation() == identity) {
    return;
  }

  bool result = false;
  geom::Matrix::Decomposition decomposition;
  std::tie(result, decomposition) = entity.transformation().decompose();

  if (!result) {
    return;
  }

  uint64_t decompositionComponents = decomposition.componentsMask();

  /*
   *  Check if the only active component of the transformation is a translation.
   */
  if (decompositionComponents !=
      static_cast<uint64_t>(
          geom::Matrix::Decomposition::Component::Translation)) {
    return;
  }

  auto frame = entity.frame();
  frame.origin.x += decomposition.translation.x;
  frame.origin.y += decomposition.translation.y;
  entity.setFrame(frame);
  entity.setTransformation(identity);
}

static void FixupBounds(interface::ModelEntity& entity) {}

static void FixupHierarchy(interface::ModelEntity& entity) {
  /*
   *  If the node has a transform that can be expressed as a frame offset, apply
   *  it to the frame and modify the transform accordingly.
   */
  FixupTransformation(entity);

  /*
   *  Make the bounds large enough for this node to encapsulate its children.
   */
  FixupBounds(entity);
}

interface::ModelEntity::Ref SVGArchive::visitNodeChildren(
    const pugi::xml_node& node,
    interface::Interface& interface,
    InterfaceBuilderArchive::EntityMap& map) const {
  if (node.empty()) {
    return nullptr;
  }

  auto entity =
      configureNode(CreateRegisteredEntity(interface, map, node), node);

  if (entity == nullptr) {
    return nullptr;
  }

  bool present = false;

  /*
   *  Fill
   */
  auto fill = Decode<entity::Color>(node, "fill", &present);
  if (present) {
    entity->setBackgroundColor(fill);
  }

  /*
   *  Fill Opacity
   */
  auto fillOpacity = Decode<double>(node, "fill-opacity", &present);
  if (present) {
    entity->setOpacity(fillOpacity);
  }

  /*
   *  Transform
   */
  auto transfrom = Decode<geom::Matrix>(node, "transform", &present);
  if (present) {
    entity->setTransformation(transfrom);
  }

  /*
   *  Children
   */
  for (const auto& childNode : node.children()) {
    entity->addChild(visitNodeChildren(childNode, interface, map));
  }

  /*
   *  We are done inflating this sub-hierarchy. Make the hierarchy consistent.
   */
  FixupHierarchy(*entity);

  return entity;
}

interface::ModelEntity::Ref SVGArchive::configureNode(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  if (entity == nullptr) {
    return nullptr;
  }

  if (::strncmp(node.name(), "rect", sizeof("rect")) == 0) {
    return configureRect(entity, node);
  }

  if (::strncmp(node.name(), "ellipse", sizeof("ellipse")) == 0) {
    return configureEllipse(entity, node);
  }

  if (::strncmp(node.name(), "g", sizeof("g")) == 0) {
    return configureG(entity, node);
  }

  if (::strncmp(node.name(), "circle", sizeof("circle")) == 0) {
    return configureCircle(entity, node);
  }

  if (::strncmp(node.name(), "polygon", sizeof("polygon")) == 0) {
    return configurePolygon(entity, node);
  }

  if (::strncmp(node.name(), "polyline", sizeof("polyline")) == 0) {
    /*
     *  A polyline is just a polygon without a fill.
     */
    return configurePolygon(entity, node);
  }

  if (::strncmp(node.name(), "line", sizeof("line")) == 0) {
    return configureLine(entity, node);
  }

  if (::strncmp(node.name(), "use", sizeof("use")) == 0) {
    return configureUse(entity, node);
  }

  if (::strncmp(node.name(), "text", sizeof("text")) == 0) {
    return configureText(entity, node);
  }

  if (::strncmp(node.name(), "path", sizeof("path")) == 0) {
    return configurePath(entity, node);
  }

  if (::strncmp(node.name(), "image", sizeof("image")) == 0) {
    return configureImage(entity, node);
  }

  if (::strncmp(node.name(), "mask", sizeof("mask")) == 0) {
    return configureMask(entity, node);
  }

  if (::strncmp(node.name(), "svg", sizeof("svg")) == 0) {
    return configureSVG(entity, node);
  }

  if (::strncmp(node.name(), "desc", sizeof("desc")) == 0) {
    return nullptr;
  }

  if (::strncmp(node.name(), "title", sizeof("title")) == 0) {
    return nullptr;
  }

  if (::strncmp(node.name(), "defs", sizeof("defs")) == 0) {
    /*
     *  Definitions have been parsed ahead of time.
     */
    return nullptr;
  }

  RL_LOG("Unknown: %s", node.name());
  return nullptr;
}

interface::ModelEntity::Ref SVGArchive::configureSVG(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  entity->setFrame(Decode<geom::Rect>(node, "viewBox"));
  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#RectElement
 */
interface::ModelEntity::Ref SVGArchive::configureRect(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
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

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#EllipseElement
 */
interface::ModelEntity::Ref SVGArchive::configureEllipse(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
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

  entity->setPath(builder.path());

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/struct.html#GElement
 */
interface::ModelEntity::Ref SVGArchive::configureG(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#CircleElement
 */
interface::ModelEntity::Ref SVGArchive::configureCircle(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
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

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#PolygonElement
 */
interface::ModelEntity::Ref SVGArchive::configurePolygon(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  /*
   *  Treat the path value as an SVG path without the preceding absolute line
   *  declaration "L".
   *  https://www.w3.org/TR/SVG11/paths.html#PathDataLinetoCommands
   */

  const auto& attribute = node.attribute("points");

  if (attribute.empty()) {
    return nullptr;
  }

  std::stringstream stream;
  stream << "L" << attribute.as_string();
  auto path = svg::SVGPathStringParse(stream.str());

  if (path.componentCount() == 0) {
    return nullptr;
  }

  entity->setPath(std::move(path));

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/shapes.html#LineElement
 */
interface::ModelEntity::Ref SVGArchive::configureLine(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
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

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG11/struct.html#UseElement
 */
interface::ModelEntity::Ref SVGArchive::configureUse(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
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

  entity = configureNode(entity, found->second);

  if (entity == nullptr) {
    return nullptr;
  }

  /*
   *  The ‘use’ element has optional attributes ‘x’, ‘y’, ‘width’ and ‘height’.
   *  If on configureing this node, we inflated a model entity, attach these
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

/**
 *  https://www.w3.org/TR/SVG/paths.html#PathElement
 */
interface::ModelEntity::Ref SVGArchive::configurePath(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  auto pathString = Decode<std::string>(node, "d");

  if (pathString.size() == 0) {
    return nullptr;
  }

  geom::Path path = svg::SVGPathStringParse(pathString);

  if (path.componentCount() == 0) {
    return nullptr;
  }

  entity->setPath(std::move(path));

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG/struct.html#ImageElement
 */
interface::ModelEntity::Ref SVGArchive::configureImage(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  auto width = Decode<double>(node, "width");

  if (width <= 0) {
    return nullptr;
  }

  auto height = Decode<double>(node, "height");

  if (height <= 0) {
    return nullptr;
  }

  auto image = Decode<image::Image>(node, "xlink:href");

  if (!image.isValid()) {
    return nullptr;
  }

  entity->setFrame(
      {Decode<double>(node, "x"), Decode<double>(node, "y"), width, height});
  entity->setContents(std::move(image));

  return entity;
}

/*
 *  https://www.w3.org/TR/SVG/masking.html#MaskElement
 */
interface::ModelEntity::Ref SVGArchive::configureMask(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  /*
   *  TODO: Wire up mask elements when the clip stack work is done.
   */
  return nullptr;
}

/*
 *  https://www.w3.org/TR/SVG/text.html#TextElement
 */
interface::ModelEntity::Ref SVGArchive::configureText(
    interface::ModelEntity::Ref entity,
    const pugi::xml_node& node) const {
  /*
   *  TODO: Wire up text elements when libTypography work items are completed.
   */
  return nullptr;
}

}  // namespace ib
}  // namespace rl
