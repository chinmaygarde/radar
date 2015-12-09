// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Suggestion.h>
#include <Layout/Utilities.h>

namespace rl {
namespace layout {

Suggestion::Suggestion() : _variable(), _value(0.0) {}

Suggestion::Suggestion(const Variable& variable, double value)
    : _variable(variable), _value(value) {}

const Variable& Suggestion::variable() const {
  return _variable;
}

double Suggestion::value() const {
  return _value;
}

bool Suggestion::serialize(core::Message& message) const {
  auto result = true;
  result &= message.encode(_variable);
  result &= message.encode(_value);
  return result;
}

bool Suggestion::deserialize(core::Message& message) {
  auto result = true;
  result &= message.decode(_variable);
  result &= message.decode(_value);
  return result;
}

std::vector<Suggestion> Suggestion::Anchor(interface::Entity& entity) {
  using Property = interface::Entity::Property;

  std::vector<Property> properties = {Property::Position, Property::Bounds};
  std::vector<Suggestion> suggestions;

  for (const auto& property : properties) {
    auto propertySuggestions = Anchor(entity, property);
    std::move(propertySuggestions.begin(), propertySuggestions.end(),
              std::back_inserter(suggestions));
  }

  return suggestions;
}

std::vector<Suggestion> Suggestion::Anchor(
    interface::Entity& entity,
    interface::Entity::Property property) {
  using Entity = interface::Entity;

  std::vector<Suggestion> suggestions;
  auto identifier = entity.identifier();

  /*
   *  WIP: Add constraint dimensions
   */

  switch (property) {
    case Entity::Property::Bounds: {
      auto bounds = entity.bounds();
      if (!NearZero(bounds.size.width)) {
        Variable width(identifier, property);
        suggestions.push_back({width, bounds.size.width});
      }
    } break;
    case interface::Entity::Property::Position: {
      auto position = entity.position();
      if (!NearZero(position.x)) {
        Variable positionX(identifier, property);
        suggestions.push_back({positionX, position.x});
      }
    } break;
    default:
      break;
  }

  return std::move(suggestions);
}

}  // namespace layout
}  // namespace rl
