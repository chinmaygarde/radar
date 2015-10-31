// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Variable.h>
#include <Recognition/ActiveTouchSet.h>

namespace rl {
namespace recognition {

Variable::Variable()
    : _identifier(interface::Entity::IdentifierNone),
      _property(interface::Entity::Property::None),
      _isProxy(false) {
}

Variable::Variable(Variable::Proxy proxy, interface::Entity::Property property)
    : _identifier(static_cast<interface::Entity::Identifier>(proxy)),
      _property(property),
      _isProxy(true) {
}

Variable::Variable(const interface::Entity& entity,
                   interface::Entity::Property property)
    : _identifier(entity.identifier()), _property(property), _isProxy(false) {
}

interface::Entity::Identifier Variable::targetIdentifier() const {
  return _identifier;
}

interface::Entity::Property Variable::targetProperty() const {
  return _property;
}

bool Variable::isProxy() const {
  return _isProxy;
}

Variable::ValueType Variable::valueType() const {
  using Property = interface::Entity::Property;
  switch (_property) {
    case Property::Position:
      return ValueType::Point;
    case Property::AnchorPoint:
      return ValueType::Point;
    case Property::Bounds:
      return ValueType::Rect;
    case Property::Transformation:
      return ValueType::Matrix;
    case Property::BackgroundColor:
      return ValueType::Color;
    case Property::Opacity:
      return ValueType::Number;
    default:
      return ValueType::Unsupported;
  }
  return ValueType::Unsupported;
}

interface::Entity& Variable::entityRepresentation(
    const ActiveTouchSet& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) const {
  if (_isProxy) {
    auto entity =
        touches.touchEntityForProxy(static_cast<Variable::Proxy>(_identifier));
    RL_ASSERT_MSG(entity != nullptr,
                  "A valid entity for the proxy was not found. Some bounds "
                  "checking earlier has gone wrong");
    return *entity;
  } else {
    auto& entity = entities.at(_identifier);
    RL_ASSERT(entity != nullptr);
    return *entity.get();
  }
}

coordinator::PresentationEntity& Variable::presentationEntityRepresentation(
    const coordinator::PresentationEntity::IdentifierMap& presentationEntities)
    const {
  RL_ASSERT_MSG(
      !_isProxy,
      "Asked for the presentation entity representation of a proxy variable");
  auto& presentationEntity = presentationEntities.at(_identifier);
  RL_ASSERT(presentationEntity != nullptr);
  return *presentationEntity.get();
}

bool Variable::serialize(core::Message& message) const {
  auto result = true;
  result &= message.encode(_identifier);
  result &= message.encode(_property);
  result &= message.encode(_isProxy);
  return result;
}

bool Variable::deserialize(core::Message& message) {
  auto result = true;
  result &= message.decode(_identifier);
  result &= message.decode(_property);
  result &= message.decode(_isProxy);
  return result;
}

}  // namespace recognition
}  // namespace rl
