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
  static const double AlphaThreshold;

  Primitive();

  ~Primitive();

  Primitive(Primitive&& primitive);

  void setSize(geom::Size size);

  void setModelViewMatrix(geom::Matrix modelViewMatrix);

  void setSolidColor(entity::Color solidColor);

  void setOpacity(double opacity);

  bool isRenderable() const;

  bool render(Frame& frame);

 private:
  geom::Size _size;
  geom::Matrix _modelViewMatrix;
  entity::Color _solidColor;
  double _opacity;

  RL_DISALLOW_COPY_AND_ASSIGN(Primitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_PRIMITIVE_
