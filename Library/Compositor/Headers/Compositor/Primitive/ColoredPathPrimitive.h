// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_COLOREDPATHPRIMITIVE_H_
#define RADAR_COMPOSITOR_COLOREDPATHPRIMITIVE_H_

#include <Core/Macros.h>
#include <Compositor/Primitive/Primitive.h>
#include <Geometry/Path.h>

namespace rl {
namespace compositor {

class Mesh;

class ColoredPathPrimitive : public Primitive {
 public:
  ColoredPathPrimitive(const geom::Path& path);

  ~ColoredPathPrimitive() override;

 private:
  std::unique_ptr<Mesh> _mesh;

  void prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

  RL_DISALLOW_COPY_AND_ASSIGN(ColoredPathPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_COLOREDPATHPRIMITIVE_H_
