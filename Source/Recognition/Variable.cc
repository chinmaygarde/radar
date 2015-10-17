// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Variable.h>
#include <Recognition/ActiveTouchSet.h>

namespace rl {

Variable::Variable()
    : _identifier(Entity::IdentifierNone),
      _property(Entity::Property::None),
      _isProxy(false) {
}

Variable::Variable(Variable::Proxy proxy, Entity::Property property)
    : _identifier(static_cast<Entity::Identifier>(proxy)),
      _property(property),
      _isProxy(true) {
}

Variable::Variable(const Entity& entity, Entity::Property property)
    : _identifier(entity.identifier()), _property(property), _isProxy(false) {
}

Entity::Identifier Variable::targetIdentifier() const {
  return _identifier;
}

Entity::Property Variable::targetProperty() const {
  return _property;
}

bool Variable::isProxy() const {
  return _isProxy;
}

Variable::ValueType Variable::valueType() const {
  switch (_property) {
    case Entity::Property::Position:
      return ValueType::Point;
    case Entity::Property::AnchorPoint:
      return ValueType::Point;
    case Entity::Property::Bounds:
      return ValueType::Rect;
    case Entity::Property::Transformation:
      return ValueType::Matrix;
    case Entity::Property::BackgroundColor:
      return ValueType::Color;
    case Entity::Property::Opacity:
      return ValueType::Number;
    default:
      return ValueType::Unsupported;
  }
  return ValueType::Unsupported;
}

Entity& Variable::entityRepresentation(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) {
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

bool Variable::serialize(Message& message) const {
  auto result = true;
  result &= message.encode(_identifier);
  result &= message.encode(_property);
  result &= message.encode(_isProxy);
  return result;
}

bool Variable::deserialize(Message& message) {
  auto result = true;
  result &= message.decode(_identifier);
  result &= message.decode(_property);
  result &= message.decode(_isProxy);
  return result;
}

}  // namespace rl
