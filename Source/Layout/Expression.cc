// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Expression.h>

namespace rl {
namespace layout {

Expression::Expression(const Terms& terms, double constant)
    : _terms(terms), _constant(constant) {
}

Expression::Expression(const Term& term) : _terms({term}), _constant(0.0) {
}

Expression::Expression(const Variable& variable)
    : _terms({Term{variable, 1.0}}), _constant(0.0) {
}

Expression::Expression(double constant) : _constant(constant) {
}

const Expression::Terms& Expression::terms() const {
  return _terms;
}

double Expression::constant() const {
  return _constant;
}

}  // namespace layout
}  // namespace rl
