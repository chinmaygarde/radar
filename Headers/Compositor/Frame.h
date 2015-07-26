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

namespace rl {
class Frame {
 public:
  Frame(Size size, std::shared_ptr<ProgramCatalog> catalog);

  /**
   *  Get the projection matrix of the current frame
   *
   *  @return the projection matrix
   */
  const Matrix& projectionMatrix() { return _projectionMatrix; }

  /**
   *  Access the preconfigured catalog of programs for this frame
   *
   *  @return the program catalog
   */
  std::shared_ptr<ProgramCatalog> programCatalog() { return _programCatalog; }

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

  void startNewFrame();

  DISALLOW_COPY_AND_ASSIGN(Frame);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_FRAME__) */
