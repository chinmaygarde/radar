// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_FRAME_
#define RADARLOVE_COMPOSITOR_FRAME_

#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Compositor/ProgramCatalog.h>
#include <Compositor/CoordinatorStatistics.h>

namespace rl {
namespace compositor {

class BoxVertices;

class Frame {
 public:
  explicit Frame(geom::Size size,
                 std::shared_ptr<ProgramCatalog> catalog,
                 CoordinatorStatistics& stats);

  virtual ~Frame();

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
   *  Access the preconfigured catalog of programs for this frame
   *
   *  @return the program catalog
   */
  std::shared_ptr<ProgramCatalog> programCatalog() const;

  /**
   *  Indicate that the frame has begun rendering. This allows the frame
   *  to perform initial setup operations
   */
  void begin();

  /**
   *  Indicate that the frame has finished rendering. This allows the frame to
   *  finalize resources.
   */
  void end();

  CoordinatorStatistics& statistics();

  BoxVertices& unitBoxVertices();

 private:
  geom::Size _size;
  geom::Matrix _projectionMatrix;
  CoordinatorStatistics& _stats;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  std::unique_ptr<BoxVertices> _unitBoxVertices;

  void setupFreshFrame();

  RL_DISALLOW_COPY_AND_ASSIGN(Frame);
};

class ScopedFrame : public Frame {
 public:
  template <class... T>
  ScopedFrame(T&&... args)
      : Frame(std::forward<T>(args)...) {
    begin();
  }

  ~ScopedFrame() { end(); }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ScopedFrame);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_FRAME_
