// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Entity/Entity.h>

namespace rl {
namespace entity {

Entity::Entity(core::Name identifier, UpdateCallback updateCallback)
    : _identifier(identifier),
      _bounds(geom::RectZero),
      _position(geom::PointZero),
      _anchorPoint(geom::Point(0.5, 0.5)),
      _transformation(geom::MatrixIdentity),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _updateCallback(updateCallback) {}

Entity::Entity(Entity&&) = default;

Entity::~Entity() = default;

void Entity::mergeProperties(const Entity& entity, PropertyMaskType only) {
  RL_ASSERT(_identifier == entity._identifier);

  if (only & PropertyMask::BoundsMask) {
    _bounds = entity._bounds;
  }

  if (only & PropertyMask::PositionMask) {
    _position = entity._position;
  }

  if (only & PropertyMask::AnchorPointMask) {
    _anchorPoint = entity._anchorPoint;
  }

  if (only & PropertyMask::TransformationMask) {
    _transformation = entity._transformation;
  }

  if (only & PropertyMask::BackgroundColorMask) {
    _backgroundColor = entity._backgroundColor;
  }

  if (only & PropertyMask::OpacityMask) {
    _opacity = entity._opacity;
  }

  if (only & PropertyMask::ContentsMask) {
    _contents = entity._contents;
  }
}

core::Name Entity::identifier() const {
  return _identifier;
}

geom::Rect Entity::frame() const {
  geom::Point origin(_position.x - (_bounds.size.width * _anchorPoint.x),
                     _position.y - (_bounds.size.height * _anchorPoint.y));

  return geom::Rect(origin, _bounds.size);
}

void Entity::setFrame(const geom::Rect& frame) {
  setBounds(geom::Rect(_bounds.origin, frame.size));
  setPosition(
      geom::Point(frame.origin.x + (_anchorPoint.x * frame.size.width),
                  frame.origin.y + (_anchorPoint.y * frame.size.height)));
}

const geom::Rect& Entity::bounds() const {
  return _bounds;
}

void Entity::setBounds(const geom::Rect& bounds) {
  _bounds = bounds;
  notifyInterfaceIfNecessary(Property::Bounds);
}

const geom::Point& Entity::position() const {
  return _position;
}

void Entity::setPosition(const geom::Point& position) {
  _position = position;
  notifyInterfaceIfNecessary(Property::Position);
}

const geom::Point& Entity::anchorPoint() const {
  return _anchorPoint;
}

void Entity::setAnchorPoint(const geom::Point& anchorPoint) {
  _anchorPoint = anchorPoint;
  notifyInterfaceIfNecessary(Property::AnchorPoint);
}

const geom::Matrix& Entity::transformation() const {
  return _transformation;
}

void Entity::setTransformation(const geom::Matrix& transformation) {
  _transformation = transformation;
  notifyInterfaceIfNecessary(Property::Transformation);
}

geom::Matrix Entity::modelMatrix() const {
  const geom::Point pos(_position.x - _anchorPoint.x * _bounds.size.width,
                        _position.y - _anchorPoint.y * _bounds.size.height);

  // clang-format off
  geom::Matrix translation(1.0,   0.0,   0.0, 0.0,
                     0.0,   1.0,   0.0, 0.0,
                     0.0,   0.0,   1.0, 0.0,
                     pos.x, pos.y, 0.0, 1.0);
  // clang-format on

  return translation * transformation();
}

const Color& Entity::backgroundColor() const {
  return _backgroundColor;
}

void Entity::setBackgroundColor(const Color& backgroundColor) {
  _backgroundColor = backgroundColor;
  notifyInterfaceIfNecessary(Property::BackgroundColor);
}

const double& Entity::opacity() const {
  return _opacity;
}

void Entity::setOpacity(double opacity) {
  _opacity = opacity;
  notifyInterfaceIfNecessary(Property::Opacity);
}

const image::Image& Entity::contents() const {
  return _contents;
}

void Entity::setContents(image::Image image) {
  _contents = std::move(image);
  notifyInterfaceIfNecessary(Property::Contents);
}

void Entity::notifyInterfaceIfNecessary(Property property,
                                        core::Name other) const {
  if (_updateCallback) {
    _updateCallback(*this, property, other);
  }
}

enum ArchiveKey {
  Identifier,
  Bounds,
  Position,
  AnchorPoint,
  Transformation,
  BackgroundColor,
  Opacity,
};

const core::ArchiveDef Entity::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "Entity",
    /* .autoAssignName = */ false,
    /* .members = */
    {
        ArchiveKey::Identifier,       //
        ArchiveKey::Bounds,           //
        ArchiveKey::Position,         //
        ArchiveKey::AnchorPoint,      //
        ArchiveKey::Transformation,   //
        ArchiveKey::BackgroundColor,  //
        ArchiveKey::Opacity,          //
    },
};

Entity::ArchiveName Entity::archiveName() const {
  return *_identifier.handle();
}

bool Entity::serialize(core::ArchiveItem& item) const {
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Identifier, _identifier));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Bounds, _bounds.toString()));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Position, _position.toString()));
  RL_RETURN_IF_FALSE(
      item.encode(ArchiveKey::AnchorPoint, _anchorPoint.toString()));
  RL_RETURN_IF_FALSE(
      item.encode(ArchiveKey::Transformation, _transformation.toString()));
  RL_RETURN_IF_FALSE(
      item.encode(ArchiveKey::BackgroundColor, _backgroundColor.toString()));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Opacity, _opacity));
  return true;
}

bool Entity::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  std::string decoded;

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Identifier, _identifier, ns));

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Bounds, decoded));
  _bounds.fromString(decoded);

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Position, decoded));
  _position.fromString(decoded);

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::AnchorPoint, decoded));
  _anchorPoint.fromString(decoded);

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Transformation, decoded));
  _transformation.fromString(decoded);

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::BackgroundColor, decoded));
  _backgroundColor.fromString(decoded);

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Opacity, _opacity));

  return true;
}

}  // namespace entity
}  // namespace rl
