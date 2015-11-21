// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Expression.h>

namespace rl {
namespace layout {

Expression::Expression() : _constant(0.0) {
}

Expression::Expression(const Terms& terms, double constant)
    : _terms(terms), _constant(constant) {
}

Expression::Expression(const Term& term) : _terms({term}), _constant(0.0) {
}

Expression::Expression(const Variable& variable)
    : _terms({Term{variable, 1.0}}), _constant(0.0) {
}

Expression::Expression(const Parameter& parameter)
    : _terms({Term{parameter.asVariable(), 1.0}}), _constant(0.0) {
}

Expression::Expression(double constant) : _constant(constant) {
}

const Expression::Terms& Expression::terms() const {
  return _terms;
}

double Expression::constant() const {
  return _constant;
}

bool Expression::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_terms);
  success &= message.encode(_constant);
  return success;
}

bool Expression::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_terms);
  success &= message.decode(_constant);
  return success;
}

}  // namespace layout
}  // namespace rl
