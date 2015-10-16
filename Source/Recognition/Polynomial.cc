// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Polynomial.h>

namespace rl {

Polynomial::Polynomial(Polynomial::Terms terms, double constant)
    : _terms(std::move(terms)), _constant(constant) {
}

Polynomial::Polynomial() : _constant(0.0) {
}

const Polynomial::Terms& Polynomial::terms() const {
  return _terms;
}

Term::Degree Polynomial::degree() const {
  Term::Degree degree = 0;

  for (const auto& term : _terms) {
    auto termDegree = term.degree();
    if (termDegree > degree) {
      degree = termDegree;
    }
  }

  return degree;
}

double Polynomial::constant() const {
  return _constant;
}

bool Polynomial::serialize(Message& message) const {
  auto result = true;
  result &= message.encode(_constant);
  result &= Serializable::SerializeVector(_terms, message);
  return result;
}

bool Polynomial::deserialize(Message& message) {
  auto result = true;
  result &= message.decode(_constant);
  result &= Serializable::DeserializeVector(_terms, message);
  return result;
}

}  // namespace rl
