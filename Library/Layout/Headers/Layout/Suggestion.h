/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Layout/Priority.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

class Suggestion : public core::ArchiveSerializable,
                   public core::MessageSerializable {
 public:
  Suggestion();

  Suggestion(const Variable& variable, double value, double priority);

  const Variable& variable() const;

  double value() const;

  double priority() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

  static std::vector<Suggestion> Anchor(entity::Entity& entity,
                                        entity::Entity::Property property,
                                        double priority);

  static std::vector<Suggestion> Anchor(entity::Entity& entity,
                                        double priority);

 private:
  Variable _variable;
  double _value;
  double _priority;

  RL_DISALLOW_ASSIGN(Suggestion);
};

}  // namespace layout
}  // namespace rl
