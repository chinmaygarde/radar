// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_TEXTUREDBOXPRIMITIVE_H_
#define RADAR_COMPOSITOR_TEXTUREDBOXPRIMITIVE_H_

#include <Core/Macros.h>
#include <Compositor/Primitive/Primitive.h>
#include <Image/Image.h>

namespace rl {
namespace compositor {

class Texture;

class TexturedBoxPrimitive : public Primitive {
 public:
  TexturedBoxPrimitive();

  ~TexturedBoxPrimitive() override;

  void prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

  void setImage(image::Image image);

 private:
  std::shared_ptr<Texture> _texture;

  RL_DISALLOW_COPY_AND_ASSIGN(TexturedBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTUREDBOXPRIMITIVE_H_
