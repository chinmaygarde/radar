// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Constraint.h>

namespace rl {
namespace layout {

static Constraint::Identifier LastConstraintIdentifier = 0;

Constraint::Constraint()
    : _identifier(0), _relation(Relation::EqualTo), _priority(priority::Weak) {}

Constraint::Constraint(const Expression& expression,
                       Relation relation,
                       double priority)
    : _identifier(++LastConstraintIdentifier),
      _expression(expression),
      _relation(relation),
      _priority(priority) {}

Constraint::Constraint(const Constraint& c)
    : _identifier(c._identifier),
      _expression(c._expression),
      _relation(c._relation),
      _priority(c._priority) {}

Constraint::Relation Constraint::relation() const {
  return _relation;
}

const Expression& Constraint::expression() const {
  return _expression;
}

double Constraint::priority() const {
  return _priority;
}

bool Constraint::hasProxies() const {
  for (const auto& term : _expression.terms()) {
    if (term.variable().isProxy()) {
      return true;
    }
  }

  return false;
}

bool Constraint::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_identifier);
  success &= message.encode(_expression);
  success &= message.encode(_relation);
  success &= message.encode(_priority);
  return success;
}

bool Constraint::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_identifier);
  success &= message.decode(_expression);
  success &= message.decode(_relation);
  success &= message.decode(_priority);
  return success;
}

}  // namespace layout
}  // namespace rl
