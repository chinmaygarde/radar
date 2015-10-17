// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/GestureRecognizer.h>

namespace rl {

static GestureRecognizer::Identifier LastGestureRecognizerIdentifier = 0;

GestureRecognizer::GestureRecognizer(Variable&& evaluationResult,
                                     Polynomial&& polynomial)
    : _identifier(++LastGestureRecognizerIdentifier),
      _evaluationResult(std::move(evaluationResult)),
      _polynomial(std::move(polynomial)),
      _touchCount(0),
      _preparedForUse(false) {
  RL_ASSERT_MSG(!_evaluationResult.isProxy(),
                "The evaluation result may not be a proxy");
}

GestureRecognizer::GestureRecognizer()
    : _identifier(0), _touchCount(0), _preparedForUse(false) {
}

GestureRecognizer::Identifier GestureRecognizer::identifier() const {
  return _identifier;
}

const Polynomial& GestureRecognizer::polynomial() const {
  return _polynomial;
}

const Variable& GestureRecognizer::evaluationResult() const {
  return _evaluationResult;
}

void GestureRecognizer::prepareForUse() {
  _observedEntities.clear();
  _observedProxies.clear();
  _touchCount = 0;

  for (const auto& term : _polynomial.terms()) {
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

  _preparedForUse = true;
}

bool GestureRecognizer::serialize(Message& message) const {
  auto result = true;

  result &= message.encode(_identifier);
  result &= _evaluationResult.serialize(message);
  result &= _polynomial.serialize(message);

  return result;
}

bool GestureRecognizer::deserialize(Message& message) {
  auto result = true;

  result &= message.decode(_identifier);
  result &= _evaluationResult.deserialize(message);
  result &= _polynomial.deserialize(message);

  prepareForUse();

  return result;
}

bool GestureRecognizer::isSolvable() const {
  auto evaluationResultType = _evaluationResult.valueType();
  if (evaluationResultType == Variable::ValueType::Unsupported) {
    return false;
  }

  auto polynomialType = _polynomial.valueType();
  if (polynomialType == Variable::ValueType::Unsupported) {
    return false;
  }

  return polynomialType == evaluationResultType;
}

bool GestureRecognizer::shouldBeginRecognition(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  RL_ASSERT(_preparedForUse);

  if (touches.size() < _touchCount) {
    return false;
  }

  /*
   *  Ask the evaluation result if all of the coordinates are inside its bounds
   */
  const auto& entity = entities.at(evaluationResult().targetIdentifier());

  for (size_t i = 0; i < _touchCount; i++) {
    auto point = touches.pointForIndex(i);
    RL_ASSERT(point.first);
    if (!entity->isPointInside(point.second)) {
      return false;
    }
  }

  return true;
}

GestureRecognizer::Continuation GestureRecognizer::stepRecognition(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  RL_ASSERT(_preparedForUse);

  if (touches.size() < _touchCount) {
    RL_LOG("Cancel");
    return Continuation::Cancel;
  }

  /*
   *  From the evaluation result, get the accessor. Solve the value from the
   *  polynomial and apply the update
   */
  switch (evaluationResult().targetProperty()) {
    case Entity::Property::Bounds:
      break;
    case Entity::Property::Position:
      break;
    case Entity::Property::AnchorPoint:
      break;
    case Entity::Property::Transformation:
      break;
    case Entity::Property::BackgroundColor:
      break;
    case Entity::Property::Opacity:
      break;
    default:
      RL_ASSERT_MSG(
          false, "The recognition subsystem cannot operate on this property");
      break;
  }

  RL_LOG("Continue");
  return Continuation::Continue;
}

}  // namespace rl
