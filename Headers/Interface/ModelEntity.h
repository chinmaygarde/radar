// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_MODELENTITY_H_
#define RADARLOVE_INTERFACE_MODELENTITY_H_

#include <Interface/Entity.h>

namespace rl {
namespace interface {

class ModelEntity : public Entity {
 public:
  ModelEntity(core::Name identifier);

  void addChild(const ModelEntity& child);

  void removeChild(const ModelEntity& child);

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ModelEntity);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_MODELENTITY_H_
