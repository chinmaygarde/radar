/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Image/Image.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class Texture;

class TexturedBoxPrimitive : public Primitive {
 public:
  TexturedBoxPrimitive(image::Image image);

  ~TexturedBoxPrimitive() override;

  void prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

 private:
  std::shared_ptr<Texture> _texture;

  RL_DISALLOW_COPY_AND_ASSIGN(TexturedBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl
