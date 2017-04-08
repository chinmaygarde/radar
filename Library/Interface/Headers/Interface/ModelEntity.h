/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Entity/Entity.h>

namespace rl {
namespace interface {

class ModelEntity : public entity::Entity {
 public:
  using Ref = std::shared_ptr<ModelEntity>;

  void addChild(ModelEntity::Ref child);

  void removeChild(ModelEntity::Ref child);

  const std::vector<ModelEntity::Ref>& children() const;

  void visitHierarchy(std::function<bool(ModelEntity&)> visitor);

 private:
  friend class Interface;

  std::vector<ModelEntity::Ref> _children;

  ModelEntity(core::Name identifier, UpdateCallback updateCallback);

  RL_DISALLOW_COPY_AND_ASSIGN(ModelEntity);
};

}  // namespace interface
}  // namespace rl
