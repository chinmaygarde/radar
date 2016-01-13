// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Utilities.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable() : _property(Property::None), _isProxy(false) {}

Variable::Variable(Proxy proxy, Property property)
    : _property(property), _isProxy(true) {}

Variable::Variable(core::Name identifier, Property property)
    : _identifier(identifier), _property(property), _isProxy(false) {}

core::Name Variable::identifier() const {
  return _identifier;
}

Variable::Property Variable::property() const {
  return _property;
}

bool Variable::isProxy() const {
  return _isProxy;
}

Variable::Proxy Variable::proxy() const {
  RL_ASSERT_MSG(false, "WIP");
  return Proxy::None;
}

double Variable::GetProperty(interface::Entity& entity, Property property) {
  switch (property) {
    case Property::BoundsOriginX:
      return entity.bounds().origin.x;
    case Property::BoundsOriginY:
      return entity.bounds().origin.y;
    case Property::BoundsWidth:
      return entity.bounds().size.width;
    case Property::BoundsHeight:
      return entity.bounds().size.height;
    case Property::PositionX:
      return entity.position().x;
    case Property::PositionY:
      return entity.position().y;
    case Property::AnchorPointX:
      return entity.anchorPoint().x;
    case Property::AnchorPointY:
      return entity.anchorPoint().y;
    case Property::None:
      RL_ASSERT(false);
      return 0.0;
  }
  return 0.0;
}

void Variable::SetProperty(interface::Entity& entity,
                           Property property,
                           double value) {
  switch (property) {
    case Property::BoundsOriginX: {
      auto bounds = entity.bounds();
      bounds.origin.x = value;
      entity.setBounds(bounds);
    } break;
    case Property::BoundsOriginY: {
      auto bounds = entity.bounds();
      bounds.origin.y = value;
      entity.setBounds(bounds);
    } break;
    case Property::BoundsWidth: {
      auto bounds = entity.bounds();
      bounds.size.width = value;
      entity.setBounds(bounds);
    } break;
    case Property::BoundsHeight: {
      auto bounds = entity.bounds();
      bounds.size.height = value;
      entity.setBounds(bounds);
    } break;
    case Property::PositionX: {
      auto position = entity.position();
      position.x = value;
      entity.setPosition(position);
    } break;
    case Property::PositionY: {
      auto position = entity.position();
      position.y = value;
      entity.setPosition(position);
    } break;
    case Property::AnchorPointX: {
      auto anchor = entity.anchorPoint();
      anchor.x = value;
      entity.setAnchorPoint(anchor);
    } break;
    case Property::AnchorPointY: {
      auto anchor = entity.anchorPoint();
      anchor.y = value;
      entity.setAnchorPoint(anchor);
    } break;
    case Property::None:
      RL_ASSERT(false);
      break;
  }
}

bool Variable::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_identifier);
  success &= message.encode(_property);
  success &= message.encode(_isProxy);
  return success;
}

bool Variable::deserialize(core::Message& message, core::Namespace* ns) {
  auto success = true;
  success &= message.decode(_identifier, ns);
  success &= message.decode(_property, ns);
  success &= message.decode(_isProxy, ns);
  return success;
}

}  // namespace layout
}  // namespace rl
