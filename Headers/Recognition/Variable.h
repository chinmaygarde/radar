// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_VARIABLE_H_
#define RADARLOVE_RECOGNITION_VARIABLE_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {

class Variable : public Serializable {
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

  explicit Variable();
  explicit Variable(Variable::Proxy proxy, Entity::Property property);
  explicit Variable(const Entity& entity, Entity::Property property);

  Entity::Identifier targetIdentifier() const;

  Entity::Property targetProperty() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

 private:
  Entity::Identifier _identifier;
  Entity::Property _property;
  bool _isProxy;

  RL_DISALLOW_ASSIGN(Variable);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_VARIABLE_H_
