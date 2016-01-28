// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Utilities.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable() : _property(Property::None), _proxy(Proxy::None) {}

Variable::Variable(Proxy proxy, Property property)
    : _property(property), _proxy(Proxy::None) {}

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
  success &= message.encode(static_cast<ProxyType>(_proxy));
  return success;
}

bool Variable::deserialize(core::Message& message, core::Namespace* ns) {
  ProxyType type = 0;

  auto success = true;

  success &= message.decode(_identifier, ns);
  success &= message.decode(_property, ns);
  success &= message.decode(type, ns);

  _proxy = static_cast<Proxy>(type);

  return success;
}

enum ArchiveKey {
  Property,
  Proxy,
};

const core::ArchiveDef Variable::ArchiveDefinition = {
    .autoAssignName = true,
    .className = "Variable",
    .members = {ArchiveKey::Property,  //
                ArchiveKey::Proxy},
};

Variable::ArchiveName Variable::archiveName() const {
  /*
   *  Cannot use the identifier directly because it may be dead (in case of
   *  proxies)
   */
  return core::ArchiveNameAuto;
}

bool Variable::serialize(core::ArchiveItem& item) const {
  auto result = true;
  result &= item.encodeEnum(ArchiveKey::Property, _property);
  result &= item.encodeEnum(ArchiveKey::Proxy, _proxy);
  return result;
}

bool Variable::deserialize(core::ArchiveItem& item) {
  auto result = true;
  result &= item.decodeEnum(ArchiveKey::Property, _property);
  result &= item.decodeEnum(ArchiveKey::Proxy, _proxy);
  return result;
}

}  // namespace layout
}  // namespace rl
