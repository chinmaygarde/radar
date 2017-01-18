// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_PRIMITIVE_
#define RADAR_COMPOSITOR_PRIMITIVE_

#include <Core/Macros.h>
#include <Core/ThreadGuard.h>
#include <Entity/Color.h>
#include <Geometry/Matrix.h>
#include <Geometry/Size.h>

namespace rl {
namespace compositor {

class BackEndPass;
class Frame;

class Primitive {
 public:
  static const double AlphaThreshold;

  Primitive();

  virtual ~Primitive();

  Primitive(Primitive&& primitive);

  void setSize(geom::Size size);

  void setModelViewMatrix(geom::Matrix modelViewMatrix);

  void setOpacity(double opacity);

  virtual void prepareToRender(BackEndPass& backEndPass) = 0;

  virtual bool render(Frame& frame) const = 0;

 protected:
  RL_DEBUG_THREAD_GUARD(_guard);
  geom::Size _size;
  geom::Matrix _modelViewMatrix;
  double _opacity;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Primitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_PRIMITIVE_
