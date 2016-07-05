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

class TexturedBoxPrimitive : public Primitive {
 public:
  TexturedBoxPrimitive();

  ~TexturedBoxPrimitive() override;

  bool render(Frame& frame) const override;

  void setImage(image::Image image);

 private:
  image::Image _image;

  RL_DISALLOW_COPY_AND_ASSIGN(TexturedBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTUREDBOXPRIMITIVE_H_
