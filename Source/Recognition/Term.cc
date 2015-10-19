// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Term.h>

#include <cmath>

namespace rl {

Term::Term(double coefficient, Term::Variables&& variables)
    : _coefficient(coefficient), _variables(std::move(variables)) {
}

Term::Term() : _coefficient(1.0), _variables() {
}

bool Term::serialize(Message& message) const {
  bool result = true;
  result &= message.encode(_coefficient);
  result &= SerializeVector(_variables, message);
  return result;
}

bool Term::deserialize(Message& message) {
  bool result = true;
  result &= message.decode(_coefficient);
  result &= DeserializeVector(_variables, message);
  return result;
}

bool Term::VariableDegree::serialize(Message& message) const {
  auto result = true;
  result &= variable.serialize(message);
  result &= message.encode(degree);
  return result;
}

bool Term::VariableDegree::deserialize(Message& message) {
  auto result = true;
  result &= variable.deserialize(message);
  result &= message.decode(degree);
  return result;
}

double Term::coefficient() const {
  return _coefficient;
}

Term::Degree Term::degree() const {
  Degree degree = 0;

  for (const auto& variableDegree : _variables) {
    degree += variableDegree.degree;
  }

  return degree;
}

const Term::Variables& Term::variables() const {
  return _variables;
}

Variable::ValueType Term::valueType() const {
  /*
   *  Step 1: If the term has no variable, it is a construction error. The
   *          coefficient (if present) should have been hoisted into the
   *          polynomial. Either way, unsupported.
   */
  if (_variables.size() == 0) {
    return Variable::ValueType::Unsupported;
  }

  /*
   *  Step 2: If the types of all the variables do not match, a solution is
   *          impossible to determine. Filter away these as well.
   *
   *          It is possible that in the future, we may not want to perform an
   *          exact type check. Instead, just make sure the dimensions are
   *          compatible. For example, 'Transformation.RotationZ' and 'double'
   */
  Variable::ValueType resolvedType = _variables[0].variable.valueType();

  if (resolvedType == Variable::ValueType::Unsupported) {
    return Variable::ValueType::Unsupported;
  }

  for (auto const& variableDegree : _variables) {
    auto valueType = variableDegree.variable.valueType();
    if (valueType != resolvedType) {
      return Variable::ValueType::Unsupported;
    }

    /*
     *  Step 3: For certain types, it does not make sense to raise a type by
     *          anything. We filter away such cases.
     */
    if (variableDegree.degree > 1) {
      switch (valueType) {
        case Variable::ValueType::Point:
        case Variable::ValueType::Rect:
        case Variable::ValueType::Color:
        case Variable::ValueType::Matrix:
          return Variable::ValueType::Unsupported;
          break;
        default:
          break;
      }
    }
  }

  /*
   *  Step 4: Applying a coefficient to certain properties is meaningless. We
   *          filter away such polynomials from recognition.
   */
  if (_coefficient != 1.0) {
    switch (resolvedType) {
      case Variable::ValueType::Color:
      case Variable::ValueType::Matrix:
        return Variable::ValueType::Unsupported;
        break;
      default:
        break;
    }
  }

  return resolvedType;
}

template <>
double Term::solve(const ActiveTouchSet& touches,
                   const PresentationEntity::IdentifierMap& entities) const {
  double solution = 0.0;

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);
    /*
     *  Fetch the value of the property to operate on
     */
    double value = OpacityAccessors.getter(entity);

    RL_ASSERT_MSG(item.variable.targetProperty() == Entity::Property::Opacity,
                  "Polynomial solutions on numbers may only operate on the "
                  "'opacity' property");

    solution = solution * pow(value, item.degree);
  }

  return _coefficient * solution;
}

template <>
Point Term::solve(const ActiveTouchSet& touches,
                  const PresentationEntity::IdentifierMap& entities) const {
  Point solution = PointZero;

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);
    /*
     *  Fetch the value of the property to operate on
     */
    Point value = PointZero;

    switch (item.variable.targetProperty()) {
      case Entity::Property::Position:
        value = PositionAccessors.getter(entity);
        break;
      case Entity::Property::AnchorPoint:
        value = AnchorPointAccessors.getter(entity);
        break;
      default:
        RL_ASSERT_MSG(false, "Cannot solve for a point using this property");
        break;
    }

    RL_ASSERT_MSG(item.degree == 1,
                  "Cannot raise a 'Point' by another 'Point'");
    solution = solution * value;
  }

  return solution * _coefficient;
}

template <>
Rect Term::solve(const ActiveTouchSet& touches,
                 const PresentationEntity::IdentifierMap& entities) const {
  Rect solution = RectZero;

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);
    /*
     *  Fetch the value of the property to operate on
     */
    Rect value = BoundsAccessors.getter(entity);

    /**
     *  Note: This should likely be augmented to include the computed 'frame'
     *        property for ease of use of the API
     */
    RL_ASSERT_MSG(item.variable.targetProperty() == Entity::Property::Bounds,
                  "Polynomial solutions on rectangles may only operate on the "
                  "'bounds' property");

    RL_ASSERT_MSG(item.degree == 1, "Cannot raise a 'Rect' by another 'Rect'");
    solution = solution * value;
  }

  return solution * _coefficient;
}

template <>
Color Term::solve(const ActiveTouchSet& touches,
                  const PresentationEntity::IdentifierMap& entities) const {
  auto solution = Color{};

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);
    /*
     *  Fetch the value of the property to operate on
     */
    Color value = BackgroundColorAccessors.getter(entity);
    RL_ASSERT_MSG(
        item.variable.targetProperty() == Entity::Property::BackgroundColor,
        "Polynomial solutions on colors may only operate on the 'background "
        "color' property");

    /*
     *  Apply the coefficient. The degree is guaranteed to be 1 since the
     *  recognizer would not have accepted this term for recognition otherwise
     */
    RL_ASSERT_MSG(degree() == 1, "Cannot raise a 'Color' by another 'Color'");
    RL_ASSERT_MSG(_coefficient == 1.0,
                  "Cannot multiply a 'Color' with a coefficient");
    solution = solution + value;
  }

  return solution;
}

template <>
Matrix Term::solve(const ActiveTouchSet& touches,
                   const PresentationEntity::IdentifierMap& entities) const {
  auto solution = Matrix{};

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);
    /*
     *  Fetch the value of the property to operate on
     */
    Matrix value = TransformationAccessors.getter(entity);
    RL_ASSERT_MSG(
        item.variable.targetProperty() == Entity::Property::Transformation,
        "Polynomial solutions on matrices may only operate on the "
        "'tranformation' property");

    /*
     *  Apply the coefficient. The degree is guaranteed to be 1 since the
     *  recognizer would not have accepted this term for recognition otherwise
     */
    RL_ASSERT_MSG(degree() == 1, "Cannot raise a 'Matrix' by another 'Matrix'");
    RL_ASSERT_MSG(_coefficient == 1.0,
                  "Cannot multiply a 'Matrix' with a coefficient");
    solution = solution + value;
  }

  return solution;
}

}  // namespace rl
