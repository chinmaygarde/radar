// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/GestureRecognizer.h>

namespace rl {
namespace recognition {

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

bool GestureRecognizer::serialize(core::Message& message) const {
  auto result = true;

  result &= message.encode(_identifier);
  result &= _evaluationResult.serialize(message);
  result &= _polynomial.serialize(message);

  return result;
}

bool GestureRecognizer::deserialize(core::Message& message) {
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

  auto typesSame = polynomialType == evaluationResultType;

  if (!typesSame) {
    /*
     *  If the types for the left and right hand sides dont match up, then a
     *  solution is impossible
     */
    return false;
  }

  /*
   *  The presence of certain types means that there has to be an additional
   *  restriction on the degree of the polynomial. For example, what does it
   *  even mean to raise a "point" by another "point"? Since we don't support it
   *  we will filter out such polynomials.
   */
  const auto polynomialDegree = _polynomial.degree();
  switch (polynomialType) {
    case Variable::ValueType::Number:
      return true;
    case Variable::ValueType::Rect:
      return polynomialDegree == 1;
    case Variable::ValueType::Point:
      return polynomialDegree == 1;
    case Variable::ValueType::Matrix:
      return polynomialDegree == 1;
    case Variable::ValueType::Color:
      return polynomialDegree == 1;
    case Variable::ValueType::Unsupported:
      return false;
  }

  return false;
}

bool GestureRecognizer::shouldBeginRecognition(
    const ActiveTouchSet& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) const {
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
    if (!entity->isWindowPointInside(point.second)) {
      return false;
    }
  }

  return true;
}

GestureRecognizer::Continuation GestureRecognizer::stepRecognition(
    const ActiveTouchSet& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) const {
  RL_ASSERT(_preparedForUse);

  if (touches.size() < _touchCount) {
    return Continuation::Cancel;
  }

  /*
   *  From the evaluation result, get the accessor. Solve the value from the
   *  polynomial and apply the update
   */
  auto& entity = _evaluationResult.presentationEntityRepresentation(entities);
  switch (evaluationResult().targetProperty()) {
    using Property = interface::Entity::Property;
    case Property::Bounds: {
      auto bounds = _polynomial.solve<geom::Rect>(touches, entities);
      entity.setBounds(bounds);
    } break;
    case Property::Position: {
      auto pointInWindow = _polynomial.solve<geom::Point>(touches, entities);
      entity.setPosition(pointInWindow);
    } break;
    case Property::AnchorPoint: {
      auto anchorPoint = _polynomial.solve<geom::Point>(touches, entities);
      entity.setAnchorPoint(anchorPoint);
    } break;
    case Property::Transformation: {
      auto transformation = _polynomial.solve<geom::Matrix>(touches, entities);
      entity.setTransformation(transformation);
    } break;
    case Property::BackgroundColor: {
      auto backgroundColor =
          _polynomial.solve<coordinator::Color>(touches, entities);
      entity.setBackgroundColor(backgroundColor);
    } break;
    case Property::Opacity: {
      auto opacity = _polynomial.solve<double>(touches, entities);
      entity.setOpacity(opacity);
    } break;
    default:
      RL_ASSERT_MSG(
          false, "The recognition subsystem cannot operate on this property");
      break;
  }

  return Continuation::Continue;
}

}  // namespace recognition
}  // namespace rl
