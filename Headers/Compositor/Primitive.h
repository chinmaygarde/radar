// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRIMITIVE__
#define __RADARLOVE_COMPOSITOR_PRIMITIVE__

#include "Core/Base.h"

#include "Geometry/Size.h"
#include "Geometry/Matrix.h"

#include "Compositor/Frame.h"
#include "Compositor/Color.h"

namespace rl {
class Primitive {
 public:
  Primitive();

  /**
   *  The model matrix of this primitive
   *
   *  @return the model matrix
   */
  const Matrix& modelMatrix() const;

  /**
   *  Sets the model matrix of this primitive
   *
   *  @param matrix the new model matrix
   */
  void setModelMatrix(const Matrix& matrix);

  /**
   *  The solid color of the contents of this primitive
   *
   *  @return the content color
   */
  const Color& contentColor() const;

  /**
   *  Sets the content color of this primitve
   *
   *  @param color the new content color
   */
  void setContentColor(const Color& color);

  /**
   *  Renders the primitive as part of this frame
   *
   *  @param frame the frame to render this primitive into
   */
  void render(Frame& frame);

 private:
  Matrix _modelMatrix;
  Color _contentColor;

  DISALLOW_COPY_AND_ASSIGN(Primitive);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PRIMITIVE__) */
