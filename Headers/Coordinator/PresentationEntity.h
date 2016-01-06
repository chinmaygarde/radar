// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONENTITY_
#define RADARLOVE_COORDINATOR_PRESENTATIONENTITY_

#include <Coordinator/TransferEntity.h>
#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Interface/Entity.h>

#include <map>

namespace rl {
namespace coordinator {

class Frame;
class PresentationEntity : public interface::Entity {
 public:
  using Borrowed = PresentationEntity*;
  using PresentationOrder = int64_t;
  using IdentifierMap = std::map<interface::Identifier,
                                 std::unique_ptr<PresentationEntity>,
                                 interface::Identifier::Compare>;

  explicit PresentationEntity(interface::Identifier identifier);

  const geom::Matrix& lastModelViewMatrix() const;

  ~PresentationEntity();

  PresentationOrder presentationOrder() const;

  void addChild(Borrowed entity);

  void removeChild(Borrowed entity);

  bool isWindowPointInside(const geom::Point& point) const;

  geom::Point convertPointFromWindow(const geom::Point& point) const;

  void render(Frame& frame, const geom::Matrix& viewMatrix);

 private:
  PresentationOrder _presentationOrder;
  std::vector<Borrowed> _children;
  geom::Matrix _lastModelViewMatrix;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONENTITY_
