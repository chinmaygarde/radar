// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRIMITIVE__
#define __RADARLOVE_COMPOSITOR_PRIMITIVE__

#include "Core/Base.h"

#include "Geometry/Size.h"
#include "Geometry/Matrix.h"

#include "Compositor/Frame.h"

namespace rl {
class Primitive {
 public:
  Primitive(Size size, Matrix modelViewMatrix);

  /**
   *  Update the model view matrix of this node
   *
   *  @param modelViewMatrix the model view matrix
   */
  void setModelViewMatrix(const Matrix& modelViewMatrix);

  /**
   *  Returns the current model view matrix
   *
   *  @return the model view matrix
   */
  const Matrix& modelViewMatrix() const;

  /**
   *  Update the size of the rendering primitive
   *
   *  @param size the size of the primitive
   */
  void setSize(const Size& size);

  /**
   *  Returns the size of the rendering primitive
   *
   *  @return the size of the primitive
   */
  const Size& size() const;

  void render(const Frame& frame);

 private:
  Size _size;
  Matrix _modelViewMatrix;

  DISALLOW_COPY_AND_ASSIGN(Primitive);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PRIMITIVE__) */
