// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_COLOREDBOXPRIMITIVE_H_
#define RADAR_COMPOSITOR_COLOREDBOXPRIMITIVE_H_

#include <Core/Macros.h>

#include "Primitive.h"

namespace rl {
namespace compositor {

class ColoredBoxPrimitive : public Primitive {
 public:
  ColoredBoxPrimitive(entity::Color color);

  ~ColoredBoxPrimitive() override;

  void prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

 private:
  entity::Color _color;

  RL_DISALLOW_COPY_AND_ASSIGN(ColoredBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_COLOREDBOXPRIMITIVE_H_
