// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_MODELENTITY_H_
#define RADARLOVE_INTERFACE_MODELENTITY_H_

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

#endif  // RADARLOVE_INTERFACE_MODELENTITY_H_
