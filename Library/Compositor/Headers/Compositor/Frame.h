// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Compositor/Context.h>
#include <Core/Macros.h>
#include <Geometry/Matrix.h>
#include <Geometry/Size.h>

namespace rl {
namespace compositor {

class Frame {
 public:
  Frame(geom::Size size, Context& context);

  ~Frame();

  /**
   *  Returns if the frame setup was successful
   *
   *  @return if the frame setup was successful
   */
  bool isReady() const;

  /**
   *  The size of the frame
   *
   *  @return the frame size
   */
  const geom::Size& size() const;

  /**
   *  Get the projection matrix of the current frame
   *
   *  @return the projection matrix
   */
  const geom::Matrix& projectionMatrix() const;

  /**
   *  Pop the last item off the opacity stack as the visitor backs out of the
   *  layer hierarchy.
   */
  void popOpacity();

  /**
   *  Indicate that the frame has begun rendering. This allows the frame
   *  to perform initial setup operations
   *
   *  @return if the frame setup operations succeeded.
   */
  RL_WARN_UNUSED_RESULT
  bool begin();

  /**
   *  Indicate that the frame has finished rendering. This allows the frame to
   *  finalize resources.
   *
   *  @return if the frame finalization operations succeeded.
   */
  RL_WARN_UNUSED_RESULT
  bool end();

  Context& context();

 private:
  geom::Size _size;
  geom::Matrix _projectionMatrix;
  Context& _context;

  void prepareFrame();
  void renderStatistics();

  RL_DISALLOW_COPY_AND_ASSIGN(Frame);
};

}  // namespace compositor
}  // namespace rl
