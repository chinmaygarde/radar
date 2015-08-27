// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_TRANSFERENTITY_H__
#define __RADARLOVE_COMPOSITOR_TRANSFERENTITY_H__

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {

class TransferEntity : public Entity {
 public:
  explicit TransferEntity(const Entity& entity);

 private:
  DISALLOW_COPY_AND_ASSIGN(TransferEntity);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_TRANSFERENTITY_H__
