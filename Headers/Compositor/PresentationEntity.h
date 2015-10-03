// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_PRESENTATIONENTITY_
#define RADARLOVE_COMPOSITOR_PRESENTATIONENTITY_

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
  using PresentationOrder = int64_t;
  using IdentifierMap =
      std::map<Entity::Identifier, std::unique_ptr<PresentationEntity>>;

  explicit PresentationEntity(Identifier identifier);

  const Matrix& lastModelViewMatrix() const;

  ~PresentationEntity();

  PresentationOrder presentationOrder() const;

  void addChild(Borrowed entity);

  void removeChild(Borrowed entity);

  bool isPointInside(const Point& point) const;

  void render(Frame& frame, const Matrix& viewMatrix);

 private:
  PresentationOrder _presentationOrder;
  std::vector<Borrowed> _children;
  Matrix _lastModelViewMatrix;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_PRESENTATIONENTITY_
