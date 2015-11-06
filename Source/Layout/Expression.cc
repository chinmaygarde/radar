// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Expression.h>

namespace rl {
namespace layout {

Expression::Expression(const std::list<Term>& terms, double constant)
    : _terms(terms), _constant(constant) {
}

const std::list<Term>& Expression::terms() const {
  return _terms;
}

double Expression::constant() const {
  return _constant;
}

}  // namespace layout
}  // namespace rl
