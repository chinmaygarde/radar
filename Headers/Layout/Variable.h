// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_VARIABLE_H_
#define RADARLOVE_LAYOUT_VARIABLE_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {
namespace layout {

class Variable : public core::Serializable {
 public:
  Variable();

  Variable(interface::Entity* entity, interface::Entity::Property property);

  bool applyUpdate(double value) const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

  struct Hash {
    std::size_t operator()(const Variable& v) const {
      return reinterpret_cast<size_t>(v._entity) ^ (v._property << 1);
    }
  };

  struct Equal {
    constexpr bool operator()(const Variable& lhs, const Variable& rhs) const {
      return lhs._entity == rhs._entity && lhs._property == rhs._property;
    }
  };

 private:
  interface::Entity* _entity;
  interface::Entity::Property _property;

  RL_DISALLOW_ASSIGN(Variable);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_VARIABLE_H_
