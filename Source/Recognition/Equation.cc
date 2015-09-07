// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Equation.h>

namespace rl {

Equation::Term::Term(double coefficient, Equation::Term::Variables variables)
    : _coefficient(coefficient), _variables(variables) {
}

double Equation::Term::coefficient() const {
  return _coefficient;
}

const Equation::Term::Variables& Equation::Term::variables() const {
  return _variables;
}

Equation::Equation(Equation::Terms terms) : _terms(std::move(terms)) {
}

const Equation::Terms& Equation::terms() const {
  return _terms;
}

}  // namespace rl
