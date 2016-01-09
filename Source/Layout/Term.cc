// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Term.h>

namespace rl {
namespace layout {

Term::Term() : _coefficient(1.0) {}

Term::Term(const Variable& variable, double coefficient, bool constant)
    : _variable(variable), _coefficient(coefficient), _constant(constant) {}

const Variable& Term::variable() const {
  return _variable;
}

double Term::coefficient() const {
  return _coefficient;
}

bool Term::isConstant() const {
  return _constant;
}

bool Term::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_variable);
  success &= message.encode(_coefficient);
  success &= message.encode(_constant);
  return success;
}

bool Term::deserialize(core::Message& message, core::Namespace* ns) {
  auto success = true;
  success &= message.decode(_variable, ns);
  success &= message.decode(_coefficient, ns);
  success &= message.decode(_constant, ns);
  return success;
}

}  // namespace layout
}  // namespace rl
