// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable(interface::Entity::Identifier identifier)
    : _entityIdentifier(identifier),
      _property(interface::Entity::Property::None),
      _value(0.0) {
}

bool Variable::applyUpdate(double value) const {
  return true;
}

bool Variable::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_entityIdentifier);
  success &= message.encode(_property);
  success &= message.encode(_value);
  return success;
}

bool Variable::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_entityIdentifier);
  success &= message.decode(_property);
  success &= message.decode(_value);
  return success;
}

}  // namespace layout
}  // namespace rl
