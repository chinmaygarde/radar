// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_VARIABLE_H_
#define RADARLOVE_RECOGNITION_VARIABLE_H_

#include <Core/Core.h>
#include <Interface/Entity.h>
#include <Coordinator/PresentationEntity.h>

#include <set>

namespace rl {
namespace recognition {

class ActiveTouchSet;
class Variable : public core::Serializable {
 public:
  enum class Proxy {
    Touch1,
    Touch2,
    Touch3,
    Touch4,
    Touch5,
    Touch6,
    Touch7,
    Touch8,
    Touch9,
    Touch10,
  };

  enum class ValueType {
    Unsupported,
    Number,
    Rect,
    Point,
    Matrix,
    Color,
  };

  using ProxySet = std::set<Proxy>;

  explicit Variable();
  explicit Variable(Variable::Proxy proxy,
                    interface::Entity::Property property);
  explicit Variable(const interface::Entity& entity,
                    interface::Entity::Property property);

  interface::Entity::Identifier targetIdentifier() const;

  interface::Entity::Property targetProperty() const;

  ValueType valueType() const;

  bool isProxy() const;

  interface::Entity& entityRepresentation(
      const ActiveTouchSet& touches,
      const coordinator::PresentationEntity::IdentifierMap& entities) const;

  coordinator::PresentationEntity& presentationEntityRepresentation(
      const coordinator::PresentationEntity::IdentifierMap& entities) const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

 private:
  interface::Entity::Identifier _identifier;
  interface::Entity::Property _property;
  bool _isProxy;

  RL_DISALLOW_ASSIGN(Variable);
};

}  // namespace recognition
}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_VARIABLE_H_
