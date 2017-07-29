/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include <Image/Image.h>
#include "Primitive.h"
#include "Texture.h"
#include "Vertices/FillVertices.h"

namespace rl {
namespace compositor {

class TexturedPathPrimitive : public Primitive {
 public:
  TexturedPathPrimitive(image::Image image, const geom::Path& path);

  ~TexturedPathPrimitive() override;

 private:
  FillVertices _vertices;
  std::shared_ptr<Texture> _texture;

  bool prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

  RL_DISALLOW_COPY_AND_ASSIGN(TexturedPathPrimitive);
};

}  // namespace compositor
}  // namespace rl
