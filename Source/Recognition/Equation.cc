// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Equation.h>

namespace rl {

Equation::Equation(Equation::Terms terms, double constant)
    : _terms(std::move(terms)), _constant(constant) {
}

const Equation::Terms& Equation::terms() const {
  return _terms;
}

Term::Degree Equation::degree() const {
  Term::Degree degree = 0;

  for (const auto& term : _terms) {
    auto termDegree = term.degree();
    if (termDegree > degree) {
      degree = termDegree;
    }
  }

  return degree;
}

bool Equation::serialize(Message& message) const {
  bool result = true;
  result &= Serializable::SerializeVector(_terms, message);
  return result;
}

bool Equation::deserialize(Message& message) {
  bool result = true;
  result &= Serializable::DeserializeVector(_terms, message);
  return result;
}

}  // namespace rl
