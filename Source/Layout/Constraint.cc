// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Constraint.h>
#include <Layout/ConstraintCreation.h>
#include <Layout/Utilities.h>

namespace rl {
namespace layout {

static Constraint::Identifier LastConstraintIdentifier = 0;

Constraint::Constraint()
    : _identifier(0), _relation(Relation::EqualTo), _priority(priority::Weak) {}

Constraint::Constraint(const Expression& expression,
                       Relation relation,
                       double priority)
    : _identifier(++LastConstraintIdentifier),
      _expression(expression),
      _relation(relation),
      _priority(priority) {}

Constraint::Constraint(const Constraint& c)
    : _identifier(c._identifier),
      _expression(c._expression),
      _relation(c._relation),
      _priority(c._priority) {}

Constraint::Relation Constraint::relation() const {
  return _relation;
}

const Expression& Constraint::expression() const {
  return _expression;
}

double Constraint::priority() const {
  return _priority;
}

std::vector<Constraint> Constraint::AnchorConstraints(
    interface::Entity& entity) {
  using Property = interface::Entity::Property;
  std::vector<Property> properties = {Property::Position, Property::Bounds};

  std::vector<Constraint> constraints;
  for (const auto& property : properties) {
    auto propertyConstraints = AnchorConstraints(entity, property);
    std::move(propertyConstraints.begin(), propertyConstraints.end(),
              std::back_inserter(constraints));
  }

  return constraints;
}

std::vector<Constraint> Constraint::AnchorConstraints(
    interface::Entity& entity,
    interface::Entity::Property property) {
  using Entity = interface::Entity;

  std::vector<Constraint> constraints;
  auto identifier = entity.identifier();

  /*
   *  WIP: Add constraint dimensions
   */

  switch (property) {
    case Entity::Property::Bounds: {
      auto bounds = entity.bounds();
      if (!NearZero(bounds.size.width)) {
        Variable width(identifier, property);
        constraints.push_back(width == bounds.size.width);
      }
    } break;
    case interface::Entity::Property::Position: {
      auto position = entity.position();
      if (!NearZero(position.x)) {
        Variable positionX(identifier, property);
        constraints.push_back(positionX == position.x);
      }
    } break;
    default:
      break;
  }

  return std::move(constraints);
}

bool Constraint::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_identifier);
  success &= message.encode(_expression);
  success &= message.encode(_relation);
  success &= message.encode(_priority);
  return success;
}

bool Constraint::deserialize(core::Message& message) {
  auto success = true;
  success &= message.decode(_identifier);
  success &= message.decode(_expression);
  success &= message.decode(_relation);
  success &= message.decode(_priority);
  return success;
}

}  // namespace layout
}  // namespace rl
