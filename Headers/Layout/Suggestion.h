// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_SUGGESTION_H_
#define RADARLOVE_LAYOUT_SUGGESTION_H_

#include <Core/Core.h>
#include <Layout/Priority.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

class Suggestion : public core::Serializable {
 public:
  Suggestion();

  Suggestion(const Variable& variable, double value, double priority);

  const Variable& variable() const;

  double value() const;

  double priority() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

  static std::vector<Suggestion> Anchor(interface::Entity& entity,
                                        interface::Entity::Property property,
                                        double priority);

  static std::vector<Suggestion> Anchor(interface::Entity& entity,
                                        double priority);

 private:
  Variable _variable;
  double _value;
  double _priority;

  RL_DISALLOW_ASSIGN(Suggestion);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SUGGESTION_H_