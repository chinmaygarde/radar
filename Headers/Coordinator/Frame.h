// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_FRAME_
#define RADARLOVE_COORDINATOR_FRAME_

#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Coordinator/ProgramCatalog.h>
#include <Coordinator/Statistics.h>
#include <Coordinator/PresentationGraph.h>

#include <mutex>
#include <deque>

namespace rl {
namespace coordinator {

class Frame {
 public:
  explicit Frame(geom::Size size,
                 std::shared_ptr<ProgramCatalog> catalog,
                 Statistics& stats);

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

  Statistics& statistics();

 private:
  geom::Size _size;
  geom::Matrix _projectionMatrix;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  Statistics& _stats;

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

  ~ScopedFrame();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ScopedFrame);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_FRAME_
