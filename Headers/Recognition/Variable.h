// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_RECOGNITION_VARIABLE_H__
#define __RADARLOVE_RECOGNITION_VARIABLE_H__

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {

class Variable {
 public:
  enum class Proxy {
    AllTouches,
    Touch1,
    Touch2,
    Touch3,
    Touch4,
    Touch5,
    Touch6,
    Touch7,
    Touch8,
    Touch9,
    Touch10,
  };

  Variable(Variable::Proxy proxy, Entity::Property property);
  Variable(const Entity& entity, Entity::Property property);
  Variable(Variable&& variable) = default;

  Entity::Identifier targetIdentifier() const;

  Entity::Property targetProperty() const;

 private:
  Entity::Identifier _identifier;
  Entity::Property _property;
  bool _isProxy;

  RL_DISALLOW_COPY_AND_ASSIGN(Variable);
};

}  // namespace rl

#endif  // __RADARLOVE_RECOGNITION_VARIABLE_H__
