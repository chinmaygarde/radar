// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_COLOREDPATHPRIMITIVE_H_
#define RADAR_COMPOSITOR_COLOREDPATHPRIMITIVE_H_

#include <Core/Macros.h>
#include <Compositor/Primitive/Primitive.h>

namespace rl {
namespace compositor {

class ColoredPathPrimitive : public Primitive {
 public:
  ColoredPathPrimitive();

  ~ColoredPathPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ColoredPathPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_COLOREDPATHPRIMITIVE_H_
