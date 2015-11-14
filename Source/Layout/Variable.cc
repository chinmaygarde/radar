// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable()
    : _entity(nullptr),
      _property(interface::Entity::Property::None),
      _value(0.0) {
}

Variable::Variable(interface::Entity* entity,
                   interface::Entity::Property property,
                   double value)
    : _entity(entity), _property(property), _value(value) {
}

bool Variable::applyUpdate(double value) const {
  return true;
}

bool Variable::serialize(core::Message& message) const {
  // TODO: Transfer entity representation
  auto success = true;
  success &= message.encode(_property);
  success &= message.encode(_value);
  return success;
}

bool Variable::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_property);
  success &= message.decode(_value);
  return success;
}

}  // namespace layout
}  // namespace rl
