// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_FRAME__
#define __RADARLOVE_COMPOSITOR_FRAME__

#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Compositor/ProgramCatalog.h>
#include <Compositor/EntityArena.h>

#include <mutex>
#include <deque>

namespace rl {
class Frame {
 public:
  Frame(Size size, std::shared_ptr<ProgramCatalog> catalog);

  virtual ~Frame();

  /**
   *  Returns if the frame setup was successful
   *
   *  @return if the frame setup was successful
   */
  bool isReady() const;

  /**
   *  Get the projection matrix of the current frame
   *
   *  @return the projection matrix
   */
  const Matrix& projectionMatrix() const;

  /**
   *  Get the current view matrix of the frame. As the frame visits different
   *  parts of the layer hierarchy, this matrix stack is updated to point to
   *  the matrix at the currently visited level
   *
   *  @return the view matrix
   */
  const Matrix& viewMatrix() const;

  /**
   *  Push a new view matrix onto the matrix stack as the depth of the layer in
   *  the hierarchy deepens
   *
   *  @param matrix the new
   */
  void pushViewMatrix(const Matrix& matrix);

  /**
   *  Pop the view matrix from the matrix stack as the visitor backs out
   */
  void popViewMatrix();

  /**
   *  Get the effective opacity of the frame at the current level in the layer
   *  hierarchy
   *
   *  @return current opacity
   */
  double opacity() const;

  /**
   *  Push a new opacity onto the opacity stack as layers deeper in the
   *  hierarchy are visited
   *
   *  @param opacity the new opacity
   */
  void pushOpacity(double opacity);

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

  /**
   *  Render the entities in the specifed arena
   *
   *  @param arena the arena containing the entities to render
   */
  void render(const EntityArena& arena);

 private:
  Size _size;
  Matrix _projectionMatrix;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  std::deque<Matrix> _viewMatrixStack;
  std::deque<double> _opacityStack;

  void setupFreshFrame();

  DISALLOW_COPY_AND_ASSIGN(Frame);
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
  DISALLOW_COPY_AND_ASSIGN(ScopedFrame);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_FRAME__) */
