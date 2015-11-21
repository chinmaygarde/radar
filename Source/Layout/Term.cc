// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Term.h>

namespace rl {
namespace layout {

Term::Term() : _variable(nullptr), _coefficient(1.0) {
}

Term::Term(const Variable& variable, double coefficient)
    : _variable(variable), _coefficient(coefficient) {
}

const Variable& Term::variable() const {
  return _variable;
}

double Term::coefficient() const {
  return _coefficient;
}

bool Term::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_variable);
  success &= message.encode(_coefficient);
  return success;
}

bool Term::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_variable);
  success &= message.decode(_coefficient);
  return success;
}

}  // namespace layout
}  // namespace rl
