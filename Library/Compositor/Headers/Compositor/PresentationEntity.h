// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_PRESENTATIONENTITY_
#define RADAR_COMPOSITOR_PRESENTATIONENTITY_

#include <Compositor/FrontendPass.h>
#include <Core/Macros.h>
#include <Entity/Entity.h>
#include <Geometry/Geometry.h>
#include <map>

namespace rl {
namespace compositor {

class PrimitivesCache;

class PresentationEntity : public entity::Entity {
 public:
  using Borrowed = PresentationEntity*;

  PresentationEntity(core::Name identifier);

  const geom::Matrix& lastModelViewMatrix() const;

  ~PresentationEntity();

  void addChild(Borrowed entity);

  void removeChild(Borrowed entity);

  bool isWindowPointInside(const geom::Point& point) const;

  geom::Point convertPointFromWindow(const geom::Point& point) const;

  void render(FrontEndPass& frontEndPass, const geom::Matrix& viewMatrix);

 private:
  geom::Matrix _renderedModelViewMatrix;
  std::vector<Borrowed> _children;
  std::unique_ptr<PrimitivesCache> _primitivesCache;

  void renderContents(FrontEndPass& frontEndPass) const;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_PRESENTATIONENTITY_
