// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/Term.h>

#include <cmath>

namespace rl {
namespace recognition {

Term::Term(double coefficient, Term::Variables&& variables)
    : _coefficient(coefficient), _variables(std::move(variables)) {
}

Term::Term() : _coefficient(1.0), _variables() {
}

bool Term::serialize(core::Message& message) const {
  bool result = true;
  result &= message.encode(_coefficient);
  result &= SerializeVector(_variables, message);
  return result;
}

bool Term::deserialize(core::Message& message) {
  bool result = true;
  result &= message.decode(_coefficient);
  result &= DeserializeVector(_variables, message);
  return result;
}

bool Term::VariableDegree::serialize(core::Message& message) const {
  auto result = true;
  result &= variable.serialize(message);
  result &= message.encode(degree);
  return result;
}

bool Term::VariableDegree::deserialize(core::Message& message) {
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
     *  Step 3: For certain types, it does not make sense to raise the value of
     *          that type by anything. We filter away such cases.
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

  /*
   *  Step 5: Certain terms require that only a one term be present. This
   *          may be because "multiplying" multiple terms may not make sense.
   *          Filter these away as well
   */
  if (_variables.size() > 1) {
    switch (resolvedType) {
      case Variable::ValueType::Color:
      case Variable::ValueType::Matrix:
        return Variable::ValueType::Unsupported;
      default:
        break;
    }
  }

  return resolvedType;
}

template <>
double Term::solve(const ActiveTouchSet& touches,
                   const PresentationEntity::IdentifierMap& entities) const {
  double solution = 1.0;

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);

    /*
     *  Fetch the value of the property to operate on
     */
    double value = entity.opacity();

    RL_ASSERT_MSG(
        item.variable.targetProperty() == interface::Entity::Property::Opacity,
        "Polynomial solutions on numbers may only operate on the "
        "'opacity' property");

    solution = solution * pow(value, item.degree);
  }

  return _coefficient * solution;
}

template <>
geom::Point Term::solve(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  auto solution = geom::Point{1.0, 1.0};

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);

    /*
     *  Fetch the value of the property to operate on
     */
    geom::Point value = geom::PointZero;

    switch (item.variable.targetProperty()) {
      case interface::Entity::Property::Position:
        value = entity.position();
        break;
      case interface::Entity::Property::AnchorPoint:
        value = entity.anchorPoint();
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
geom::Rect Term::solve(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  auto solution = geom::Rect{{1.0, 1.0}, {1.0, 1.0}};

  for (auto const& item : _variables) {
    const auto& entity = item.variable.entityRepresentation(touches, entities);

    /*
     *  Fetch the value of the property to operate on
     */
    geom::Rect value = entity.bounds();

    /**
     *  Note: This should likely be augmented to include the computed 'frame'
     *        property for ease of use of the API
     */
    RL_ASSERT_MSG(
        item.variable.targetProperty() == interface::Entity::Property::Bounds,
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
  RL_ASSERT_MSG(
      _variables.size() == 1,
      "There may only be a single variable in a term operating on colors");

  RL_ASSERT_MSG(_coefficient == 1.0,
                "Cannot multiply a 'Color' with a coefficient");

  auto const& item = _variables[0];

  const auto& entity = item.variable.entityRepresentation(touches, entities);

  /*
   *  Fetch the value of the property to operate on
   */
  auto solution = entity.backgroundColor();

  RL_ASSERT_MSG(
      item.variable.targetProperty() ==
          interface::Entity::Property::BackgroundColor,
      "Polynomial solutions on colors may only operate on the 'background "
      "color' property");

  RL_ASSERT_MSG(item.degree == 1, "Cannot raise a 'Color' by another 'Color'");

  return solution;
}

template <>
geom::Matrix Term::solve(
    const ActiveTouchSet& touches,
    const PresentationEntity::IdentifierMap& entities) const {
  RL_ASSERT_MSG(
      _variables.size() == 1,
      "There may only be a single variable in a term operating on matrices");

  RL_ASSERT_MSG(_coefficient == 1.0,
                "Cannot multiply a 'Matrix' with a coefficient");

  auto const& item = _variables[0];

  const auto& entity = item.variable.entityRepresentation(touches, entities);

  /*
   *  Fetch the value of the property to operate on
   */
  auto solution = entity.transformation();

  RL_ASSERT_MSG(item.variable.targetProperty() ==
                    interface::Entity::Property::Transformation,
                "Polynomial solutions on matrices may only operate on the "
                "'tranformation' property");

  RL_ASSERT_MSG(item.degree == 1,
                "Cannot raise a 'Matrix' by another 'Matrix'");

  return solution;
}

}  // namespace recognition
}  // namespace rl
