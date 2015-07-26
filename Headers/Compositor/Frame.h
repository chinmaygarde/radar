// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_FRAME__
#define __RADARLOVE_COMPOSITOR_FRAME__

#include "Core/Base.h"
#include "Geometry/Matrix.h"
#include "Geometry/Size.h"
#include "Compositor/ProgramCatalog.h"

#include <mutex>
#include <stack>

namespace rl {
class Frame {
 public:
  Frame(Size size, std::shared_ptr<ProgramCatalog> catalog);

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

 private:
  Size _size;
  Matrix _projectionMatrix;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  std::stack<Matrix> _viewMatrixStack;

  void startNewFrame();

  DISALLOW_COPY_AND_ASSIGN(Frame);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_FRAME__) */
