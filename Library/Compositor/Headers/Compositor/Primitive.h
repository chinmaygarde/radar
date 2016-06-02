// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_PRIMITIVE_
#define RADARLOVE_COMPOSITOR_PRIMITIVE_

#include <Core/Core.h>

#include <Geometry/Geometry.h>

#include <Entity/Color.h>
#include <Compositor/Frame.h>

namespace rl {
namespace compositor {

class Primitive {
 public:
  explicit Primitive();

  const entity::Color& color() const;

  void setColor(const entity::Color& color);

  double opacity() const;

  void setOpacity(double opacity);

  bool render(Frame& frame,
              const geom::Matrix& modelViewMatrix,
              const geom::Size& size);

 private:
  entity::Color _color;
  double _opacity;

  RL_DISALLOW_COPY_AND_ASSIGN(Primitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_PRIMITIVE_
