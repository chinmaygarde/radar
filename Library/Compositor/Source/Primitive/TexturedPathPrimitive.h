// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include <Image/Image.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class PathVertices;
class Texture;

class TexturedPathPrimitive : public Primitive {
 public:
  TexturedPathPrimitive(image::Image image, const geom::Path& path);

  ~TexturedPathPrimitive() override;

 private:
  std::shared_ptr<Texture> _texture;
  std::unique_ptr<PathVertices> _pathVertices;

  void prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

  RL_DISALLOW_COPY_AND_ASSIGN(TexturedPathPrimitive);
};

}  // namespace compositor
}  // namespace rl
