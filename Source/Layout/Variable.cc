// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Utilities.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable()
    : _identifier(interface::Entity::IdentifierNone),
      _property(Property::None),
      _isProxy(false) {}

Variable::Variable(Proxy proxy, Property property)
    : _identifier(static_cast<ProxyType>(proxy)),
      _property(property),
      _isProxy(true) {}

Variable::Variable(interface::Entity::Identifier identifier, Property property)
    : _identifier(identifier), _property(property), _isProxy(false) {}

interface::Entity::Identifier Variable::identifier() const {
  return _identifier;
}

Variable::Property Variable::property() const {
  return _property;
}

bool Variable::isProxy() const {
  return _isProxy;
}

bool Variable::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_identifier);
  success &= message.encode(_property);
  success &= message.encode(_isProxy);
  return success;
}

bool Variable::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_identifier);
  success &= message.decode(_property);
  success &= message.decode(_isProxy);
  return success;
}

}  // namespace layout
}  // namespace rl
