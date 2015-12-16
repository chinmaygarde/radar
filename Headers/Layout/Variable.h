// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_VARIABLE_H_
#define RADARLOVE_LAYOUT_VARIABLE_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {
namespace layout {

class Variable : public core::Serializable {
 public:
  using PropertyType = uint8_t;
  enum class Property : PropertyType {
    None,
    BoundsOriginX,
    BoundsOriginY,
    BoundsWidth,
    BoundsHeight,
    PositionX,
    PositionY,
    AnchorPointX,
    AnchorPointY,
  };

  using ProxyType = uint8_t;
  enum class Proxy : ProxyType {
    None,
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

  Variable();

  Variable(interface::Entity::Identifier identifier,
           Property property = Property::None);

  Variable(Proxy proxy, Property property);

  interface::Entity::Identifier identifier() const;

  Property property() const;

  bool isProxy() const;

  Proxy proxy() const;

  static double GetProperty(interface::Entity& entity, Property property);
  static void SetProperty(interface::Entity& entity,
                          Property property,
                          double value);

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

  struct Hash {
    std::size_t operator()(const Variable& v) const {
      return v._identifier ^ static_cast<PropertyType>(v._property);
    }
  };

  struct Equal {
    constexpr bool operator()(const Variable& lhs, const Variable& rhs) const {
      return lhs._identifier == rhs._identifier &&
             lhs._property == rhs._property;
    }
  };

 private:
  interface::Entity::Identifier _identifier;
  Property _property;
  bool _isProxy;
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_VARIABLE_H_
