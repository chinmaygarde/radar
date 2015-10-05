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
      _equation(std::move(equation)),
      _touchCount(0) {
  RL_ASSERT(!_evaluationResult.isProxy() &&
            "The evaluation result may not be a proxy");
  prepareForUse();
}

GestureRecognizer::GestureRecognizer() : _identifier(0), _touchCount(0) {
}

GestureRecognizer::Identifier GestureRecognizer::identifier() const {
  return _identifier;
}

const Equation& GestureRecognizer::equation() const {
  return _equation;
}

const Variable& GestureRecognizer::evaluationResult() const {
  return _evaluationResult;
}

void GestureRecognizer::prepareForUse() {
  _observedEntities.clear();
  _observedProxies.clear();
  _touchCount = 0;

  for (const auto& term : _equation.terms()) {
    for (const auto& varDegree : term.variables()) {
      auto targetIdentifier = varDegree.variable.targetIdentifier();
      if (varDegree.variable.isProxy()) {
        _observedProxies.insert(static_cast<Variable::Proxy>(targetIdentifier));

        /**
         *  Identifiers of proxies are enum indices of said proxies
         */
        if (targetIdentifier + 1 > _touchCount) {
          _touchCount = targetIdentifier + 1;
        }
      } else {
        _observedEntities.insert(targetIdentifier);
      }
    }
  }
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

  prepareForUse();

  return result;
}

bool GestureRecognizer::shouldBeginRecognition(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  if (touches.size() < _touchCount) {
    return false;
  }

  /*
   *  Ask the evaluation result if all of the coordinates are inside its bounds
   */
  const auto& entity = entities.at(evaluationResult().targetIdentifier());

  for (size_t i = 0; i < _touchCount; i++) {
    auto point = touches.pointForIndex(i);
    if (!point.first) {
      RL_ASSERT(false);
      return false;
    }

    if (!entity->isPointInside(point.second)) {
      return false;
    }
  }

  return true;
}

bool GestureRecognizer::shouldContinueRecognition(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  return false;
}

}  // namespace rl
