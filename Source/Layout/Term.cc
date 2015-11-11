// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Term.h>

namespace rl {
namespace layout {

Term::Term(const Variable& variable, double coefficient)
    : _variable(variable), _coefficient(coefficient) {
}

const Variable& Term::variable() const {
  return _variable;
}

double Term::coefficient() const {
  return _coefficient;
}

Term Term::operator*(double m) const {
  return {_variable, _coefficient * m};
}

Term Term::operator/(double m) const {
  return {_variable, _coefficient / m};
}

}  // namespace layout
}  // namespace rl
