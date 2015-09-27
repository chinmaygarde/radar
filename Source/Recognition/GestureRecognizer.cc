// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/GestureRecognizer.h>

namespace rl {

static GestureRecognizer::Identifier LastGestureRecognizerIdentifier = 0;

GestureRecognizer::GestureRecognizer(Variable&& evaluationResult,
                                     Equation&& equation)
    : _identifier(++LastGestureRecognizerIdentifier),
      _evaluationResult(std::move(evaluationResult)),
      _equation(std::move(equation)) {
}

GestureRecognizer::GestureRecognizer() : _identifier(0) {
}

const Equation& GestureRecognizer::equation() const {
  return _equation;
}

const Variable& GestureRecognizer::evaluationResult() const {
  return _evaluationResult;
}

bool GestureRecognizer::serialize(Message& message) const {
  auto result = true;
  result &= message.encode(_identifier);
  result &= _evaluationResult.serialize(message);
  result &= _equation.serialize(message);
  return result;
}

bool GestureRecognizer::deserialize(Message& message) {
  auto result = true;
  result &= message.decode(_identifier);
  result &= _evaluationResult.deserialize(message);
  result &= _equation.deserialize(message);
  return result;
}

}  // namespace rl
