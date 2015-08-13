// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONENTITY__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONENTITY__

#include <Core/Core.h>
#include <Geometry/Matrix.h>
#include <Interface/Entity.h>

namespace rl {
class PresentationEntity : Entity {
 public:
  PresentationEntity(const Entity& entity);

 private:
  ~PresentationEntity();

  DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PRESENTATIONENTITY__) */
