// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONENTITY_
#define RADARLOVE_COORDINATOR_PRESENTATIONENTITY_

#include <Coordinator/TransferEntity.h>
#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Entity/Entity.h>

#include <map>

namespace rl {
namespace coordinator {

class Frame;
class PresentationEntity : public interface::Entity {
 public:
  using Borrowed = PresentationEntity*;
  using IdentifierMap =
      std::map<core::Name, std::unique_ptr<PresentationEntity>>;
  using IdentifierHandleMap =
      std::map<core::Name::Handle, std::unique_ptr<PresentationEntity>>;

  explicit PresentationEntity(core::Name identifier);

  const geom::Matrix& lastModelViewMatrix() const;

  ~PresentationEntity();

  void addChild(Borrowed entity);

  void removeChild(Borrowed entity);

  bool isWindowPointInside(const geom::Point& point) const;

  geom::Point convertPointFromWindow(const geom::Point& point) const;

  bool render(Frame& frame, const geom::Matrix& viewMatrix);

 private:
  std::vector<Borrowed> _children;
  geom::Matrix _lastModelViewMatrix;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONENTITY_
