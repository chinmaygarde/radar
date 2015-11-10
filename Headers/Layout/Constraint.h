// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_CONSTRAINT_H_
#define RADARLOVE_LAYOUT_CONSTRAINT_H_

#include <Core/Core.h>
#include <Layout/Expression.h>

namespace rl {
namespace layout {

class Constraint {
 public:
  using Identifier = uint64_t;

  enum class Relation {
    EqualTo,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo,
  };

  Constraint(const Expression& expression, Relation relation, double priority);

  Constraint(const Constraint& constraint);

  Relation relation() const;

  const Expression& expression() const;

  double priority() const;

  struct Compare {
    constexpr bool operator()(const Constraint& lhs,
                              const Constraint& rhs) const {
      return lhs._identifier < rhs._identifier;
    }
  };

 private:
  const Identifier _identifier;
  const Relation _relation;
  const Expression _expression;
  const double _priority;

  RL_DISALLOW_ASSIGN(Constraint);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_CONSTRAINT_H_
