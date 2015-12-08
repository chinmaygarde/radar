// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Suggestion.h>

namespace rl {
namespace layout {

Suggestion::Suggestion(const Variable& variable, double value)
    : _variable(variable), _value(value) {}

const Variable& Suggestion::variable() const {
  return _variable;
}

double Suggestion::value() const {
  return _value;
}

}  // namespace layout
}  // namespace rl
