// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Variable.h>

#include "LayoutUtilities.h"

namespace rl {
namespace layout {

Variable::Variable() : _property(Property::None), _proxy(Proxy::None) {}

Variable::Variable(Proxy proxy, Property property)
    : _property(property), _proxy(proxy) {}

Variable::Variable(core::Name identifier, Property property)
    : _identifier(identifier), _property(property), _proxy(Proxy::None) {}

core::Name Variable::identifier() const {
  return _identifier;
}

Variable::Property Variable::property() const {
  return _property;
}

bool Variable::isProxy() const {
  return _proxy != Proxy::None;
}

Variable::Proxy Variable::proxy() const {
  return _proxy;
}

double Variable::GetProperty(entity::Entity& entity, Property property) {
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

void Variable::SetProperty(entity::Entity& entity,
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
  RL_RETURN_IF_FALSE(message.encode(_identifier));
  RL_RETURN_IF_FALSE(message.encode(_property));
  RL_RETURN_IF_FALSE(message.encode(static_cast<ProxyType>(_proxy)));
  return true;
}

bool Variable::deserialize(core::Message& message, core::Namespace* ns) {
  ProxyType type = 0;

  RL_RETURN_IF_FALSE(message.decode(_identifier, ns));
  RL_RETURN_IF_FALSE(message.decode(_property, ns));
  RL_RETURN_IF_FALSE(message.decode(type, ns));

  _proxy = static_cast<Proxy>(type);

  return true;
}

enum ArchiveKey {
  Identifier,
  Property,
  Proxy,
};

const core::ArchiveDef Variable::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "Variable",
    /* .autoAssignName = */ true,
    /* .members = */
    {ArchiveKey::Identifier,  //
     ArchiveKey::Property,    //
     ArchiveKey::Proxy},
};

Variable::ArchiveName Variable::archiveName() const {
  return core::ArchiveNameAuto;
}

bool Variable::serialize(core::ArchiveItem& item) const {
  RL_RETURN_IF_FALSE(
      item.encode(ArchiveKey::Identifier, _identifier.toString()));
  RL_RETURN_IF_FALSE(item.encodeEnum(ArchiveKey::Property, _property));
  RL_RETURN_IF_FALSE(item.encodeEnum(ArchiveKey::Proxy, _proxy));

  return true;
}

bool Variable::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  std::string name;

  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Identifier, name));
  RL_RETURN_IF_FALSE(item.decodeEnum(ArchiveKey::Property, _property));
  RL_RETURN_IF_FALSE(item.decodeEnum(ArchiveKey::Proxy, _proxy));

  _identifier.fromString(name, ns);

  return true;
}

}  // namespace layout
}  // namespace rl
