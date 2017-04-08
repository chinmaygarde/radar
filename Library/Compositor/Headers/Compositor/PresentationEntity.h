/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/FrontendPass.h>
#include <Core/Macros.h>
#include <Entity/Entity.h>
#include <Geometry/Matrix.h>
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
