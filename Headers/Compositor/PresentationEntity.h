// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONENTITY__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONENTITY__

#include <Core/Core.h>
#include <Interface/Entity.h>
#include <Geometry/Geometry.h>
#include <Compositor/TransferEntity.h>

#include <map>

namespace rl {
class Frame;
class PresentationEntity : public Entity {
 public:
  using Borrowed = PresentationEntity*;

  explicit PresentationEntity(Identifier identifier);

  ~PresentationEntity();

  void addChild(Borrowed entity);

  void removeChild(Borrowed entity);

  void render(Frame& frame, const Matrix& viewMatrix = MatrixIdentity) const;

 private:
  std::vector<Borrowed> _children;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};

}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PRESENTATIONENTITY__) */
