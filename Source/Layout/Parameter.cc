// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Parameter.h>

namespace rl {
namespace layout {

Parameter::Parameter(double value)
    : _identifier(interface::Entity::IdentifierNone),
      _property(interface::Entity::Property::None),
      _value(value) {
}

Parameter::Parameter(const interface::Entity& entity,
                     interface::Entity::Property property)
    : _identifier(entity.identifier()), _property(property), _value(0.0) {
}

bool Parameter::setValue(double value) {
  if (_value == value) {
    return false;
  }

  _value = value;

  return true;
}

double Parameter::value() const {
  return _value;
}

interface::Entity::Property Parameter::property() const {
  return _property;
}

Variable Parameter::asVariable() const {
  return *this;
}

}  // namespace layout
}  // namespace rl
