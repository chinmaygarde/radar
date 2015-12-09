// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Utilities.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable()
    : _identifier(interface::Entity::IdentifierNone),
      _property(Property::None) {}

Variable::Variable(interface::Entity::Identifier identifier, Property property)
    : _identifier(identifier), _property(property) {}

interface::Entity::Identifier Variable::identifier() const {
  return _identifier;
}

Variable::Property Variable::property() const {
  return _property;
}

bool Variable::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_identifier);
  success &= message.encode(_property);
  return success;
}

bool Variable::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_identifier);
  success &= message.decode(_property);
  return success;
}

}  // namespace layout
}  // namespace rl
