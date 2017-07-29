/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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

  static const double StrokeThreshold;

  Primitive();

  virtual ~Primitive();

  Primitive(Primitive&& primitive);

  void setSize(geom::Size size);

  void setModelViewMatrix(geom::Matrix modelViewMatrix);

  void setOpacity(double opacity);

  RL_WARN_UNUSED_RESULT
  virtual bool prepareToRender(BackEndPass& backEndPass) = 0;

  RL_WARN_UNUSED_RESULT
  virtual bool render(Frame& frame) const = 0;

 protected:
  RL_DEBUG_THREAD_GUARD(_guard);
  geom::Size _size;
  geom::Matrix _modelViewMatrix;
  double _opacity = 1.0;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Primitive);
};

}  // namespace compositor
}  // namespace rl
