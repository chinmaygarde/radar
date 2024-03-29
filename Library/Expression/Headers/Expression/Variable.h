/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Utilities.h>
#include <Entity/Entity.h>

namespace rl {
namespace expr {

class Variable : public core::ArchiveSerializable,
                 public core::MessageSerializable {
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

  Variable(core::Name identifier, Property property = Property::None);

  Variable(Proxy proxy, Property property);

  core::Name identifier() const;

  Property property() const;

  bool isProxy() const;

  Proxy proxy() const;

  static double GetProperty(entity::Entity& entity, Property property);

  static void SetProperty(entity::Entity& entity,
                          Property property,
                          double value);

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

  struct Hash {
    std::size_t operator()(const Variable& v) const {
      size_t seed = core::Name::Hash()(v._identifier);
      core::HashCombine(seed, static_cast<PropertyType>(v._property));
      return seed;
    }
  };

  struct Equal {
    bool operator()(const Variable& lhs, const Variable& rhs) const {
      return lhs._identifier == rhs._identifier &&
             lhs._property == rhs._property;
    }
  };

 private:
  core::Name _identifier;
  Property _property;
  Proxy _proxy;
};

}  // namespace expr
}  // namespace rl
