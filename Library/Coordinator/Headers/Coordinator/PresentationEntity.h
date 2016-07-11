// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONENTITY_
#define RADARLOVE_COORDINATOR_PRESENTATIONENTITY_

#include <Coordinator/TransferEntity.h>
#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Entity/Entity.h>
#include <Compositor/FrontendPass.h>

#include <map>

namespace rl {
namespace coordinator {

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

  void render(compositor::FrontEndPass& frontEndPass,
              const geom::Matrix& viewMatrix);

 private:
  enum class ContentType {
    None,
    SolidColor,
    Image,
  };

  enum class PrimitiveType {
    None,
    Box,
    Path,
  };

  std::vector<Borrowed> _children;
  geom::Matrix _renderedModelViewMatrix;

  ContentType contentType(double alpha) const;
  PrimitiveType primitiveType() const;
  std::shared_ptr<compositor::Primitive> solidColoredPrimitive(
      PrimitiveType type) const;
  std::shared_ptr<compositor::Primitive> imagePrimitive(
      PrimitiveType type) const;
  void renderContents(compositor::FrontEndPass& frontEndPass) const;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationEntity);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONENTITY_
