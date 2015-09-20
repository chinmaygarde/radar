// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/GestureRecognizer.h>

namespace rl {

GestureRecognizer::GestureRecognizer(Variable&& evaluationResult,
                                     Equation&& equation)
    : _evaluationResult(std::move(evaluationResult)),
      _equation(std::move(equation)) {
}

const Equation& GestureRecognizer::equation() const {
  return _equation;
}

const Variable& GestureRecognizer::evaluationResult() const {
  return _evaluationResult;
}

bool GestureRecognizer::serialize(Message& message) const {
  bool result = true;
  result &= _equation.serialize(message);
  return result;
}

bool GestureRecognizer::deserialize(Message& message) {
  bool result = true;
  result &= _equation.deserialize(message);
  return result;
}

}  // namespace rl
