// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Term.h>

namespace rl {

Term::Term(double coefficient, Term::Variables&& variables)
    : _coefficient(coefficient), _variables(std::move(variables)) {
}

Term::Term() : _coefficient(1.0), _variables() {
}

double Term::coefficient() const {
  return _coefficient;
}

Term::Degree Term::degree() const {
  Degree degree = 0;

  for (const auto& variableDegree : _variables) {
    if (variableDegree.second > degree) {
      degree = variableDegree.second;
    }
  }

  return degree;
}

const Term::Variables& Term::variables() const {
  return _variables;
}

bool Term::serialize(Message& message) const {
  bool result = true;
  result &= message.encode(_coefficient);
  return result;
}

bool Term::deserialize(Message& message) {
  bool result = true;
  result &= message.decode(_coefficient);
  return result;
}

}  // namespace rl
