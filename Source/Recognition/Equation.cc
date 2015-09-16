// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Equation.h>

namespace rl {

Equation::Term::Term(double coefficient, Equation::Term::Variables variables)
    : _coefficient(coefficient), _variables(variables) {
}

Equation::Term::Term() : _coefficient(1.0), _variables() {
}

double Equation::Term::coefficient() const {
  return _coefficient;
}

const Equation::Term::Variables& Equation::Term::variables() const {
  return _variables;
}

bool Equation::Term::serialize(Message& message) const {
  bool result = true;
  result &= message.encode(_coefficient);
  return result;
}

bool Equation::Term::deserialize(Message& message) {
  bool result = true;
  result &= message.decode(_coefficient);
  return result;
}

Equation::Equation(Equation::Terms terms) : _terms(std::move(terms)) {
}

const Equation::Terms& Equation::terms() const {
  return _terms;
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
