// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Constraint.h>

namespace rl {
namespace layout {

Constraint::Constraint(const Expression& expression,
                       Relation relation,
                       double priority)
    : _expression(expression), _relation(relation), _priority(priority) {
}

Constraint::Relation Constraint::relation() const {
  return _relation;
}

const Expression& Constraint::expression() const {
  return _expression;
}

double Constraint::priority() const {
  return _priority;
}

}  // namespace layout
}  // namespace rl
