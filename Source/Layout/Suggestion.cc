// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Suggestion.h>
#include <Layout/Utilities.h>

namespace rl {
namespace layout {

Suggestion::Suggestion() : _variable(), _value(0.0) {}

Suggestion::Suggestion(const Variable& variable, double value, double priority)
    : _variable(variable), _value(value), _priority(priority) {
  RL_ASSERT_MSG(priority != priority::Required,
                "Suggestions cannot be at required priority");
}

const Variable& Suggestion::variable() const {
  return _variable;
}

double Suggestion::value() const {
  return _value;
}

double Suggestion::priority() const {
  return _priority;
}

bool Suggestion::serialize(core::Message& message) const {
  auto result = true;
  result &= message.encode(_variable);
  result &= message.encode(_value);
  result &= message.encode(_priority);
  return result;
}

bool Suggestion::deserialize(core::Message& message) {
  auto result = true;
  result &= message.decode(_variable);
  result &= message.decode(_value);
  result &= message.decode(_priority);
  return result;
}

std::vector<Suggestion> Suggestion::Anchor(interface::Entity& entity,
                                           double priority) {
  using Property = interface::Entity::Property;

  std::vector<Property> properties = {Property::Position, Property::Bounds};
  std::vector<Suggestion> suggestions;

  for (const auto& property : properties) {
    auto propertySuggestions = Anchor(entity, property, priority);
    std::move(propertySuggestions.begin(), propertySuggestions.end(),
              std::back_inserter(suggestions));
  }

  return suggestions;
}

std::vector<Suggestion> Suggestion::Anchor(interface::Entity& entity,
                                           interface::Entity::Property property,
                                           double priority) {
  using Entity = interface::Entity;
  using Property = layout::Variable::Property;

  std::vector<Suggestion> suggestions;
  auto identifier = entity.identifier();

  /*
   *  WIP: Add constraint dimensions
   */

  switch (property) {
    case Entity::Property::Bounds: {
      auto bounds = entity.bounds();
      suggestions.push_back(
          {{identifier, Property::BoundsOriginX}, bounds.origin.x, priority});
      suggestions.push_back(
          {{identifier, Property::BoundsOriginY}, bounds.origin.y, priority});
      suggestions.push_back(
          {{identifier, Property::BoundsWidth}, bounds.size.width, priority});
      suggestions.push_back(
          {{identifier, Property::BoundsHeight}, bounds.size.height, priority});
    } break;
    case interface::Entity::Property::Position: {
      auto position = entity.position();
      suggestions.push_back(
          {{identifier, Property::PositionX}, position.x, priority});
      suggestions.push_back(
          {{identifier, Property::PositionY}, position.y, priority});
    } break;
    default:
      break;
  }

  return std::move(suggestions);
}

}  // namespace layout
}  // namespace rl
