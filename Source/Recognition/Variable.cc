// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Variable.h>

namespace rl {

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

}  // namespace rl
