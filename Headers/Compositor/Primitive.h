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
   *  Update the size and model view matrix of the primitive
   *  TODO: Remove this and make updates consistent
   *
   *  @param size            the new size
   *  @param modelViewMatrix the new model view matrix
   */
  void setSizeAndModelViewMatrix(const Size& size,
                                 const Matrix& modelViewMatrix);

  /**
   *  Returns the size of the rendering primitive
   *
   *  @return the size of the primitive
   */
  const Size& size() const;

  /**
   *  The solid color of the contents of this node
   *
   *  @return the content color
   */
  const Color& contentColor() const;

  /**
   *  Sets the content color of this node
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
  Size _size;
  Matrix _modelViewMatrix;
  Color _contentColor;
  bool _verticesDirty;

  void updateVerticesIfNecessary();

  DISALLOW_COPY_AND_ASSIGN(Primitive);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PRIMITIVE__) */
