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
    degree += variableDegree.degree;
  }

  return degree;
}

const Term::Variables& Term::variables() const {
  return _variables;
}

Variable::ValueType Term::valueType() const {
  if (_variables.size() == 0) {
    return Variable::ValueType::Unsupported;
  }

  Variable::ValueType check = _variables[0].variable.valueType();

  if (check == Variable::ValueType::Unsupported) {
    return Variable::ValueType::Unsupported;
  }

  for (auto const& variableDegree : _variables) {
    if (variableDegree.variable.valueType() != check) {
      return Variable::ValueType::Unsupported;
    }
  }

  return check;
}

bool Term::serialize(Message& message) const {
  bool result = true;
  result &= message.encode(_coefficient);
  result &= SerializeVector(_variables, message);
  return result;
}

bool Term::deserialize(Message& message) {
  bool result = true;
  result &= message.decode(_coefficient);
  result &= DeserializeVector(_variables, message);
  return result;
}

bool Term::VariableDegree::serialize(Message& message) const {
  auto result = true;
  result &= variable.serialize(message);
  result &= message.encode(degree);
  return result;
}

bool Term::VariableDegree::deserialize(Message& message) {
  auto result = true;
  result &= variable.deserialize(message);
  result &= message.decode(degree);
  return result;
}

}  // namespace rl
