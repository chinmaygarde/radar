// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_TOUCHENTITY_H_
#define RADARLOVE_RECOGNITION_TOUCHENTITY_H_

#include <Core/Core.h>
#include <Interface/Entity.h>
#include <Event/TouchEvent.h>

#include <map>

namespace rl {

class TouchEntity : public interface::Entity {
 public:
  using IdentifierMap =
      std::map<Entity::Identifier, std::unique_ptr<TouchEntity>>;

  TouchEntity(const event::TouchEvent& event);
  ~TouchEntity() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TouchEntity);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_TOUCHENTITY_H_
