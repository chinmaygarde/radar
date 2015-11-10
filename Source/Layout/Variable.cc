// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Variable.h>

namespace rl {
namespace layout {

Variable::Variable(interface::Entity* entity,
                   interface::Entity::Property property)
    : _entity(entity), _property(property) {
}

bool Variable::applyUpdate(double value) const {
  return true;
}

}  // namespace layout
}  // namespace rl
