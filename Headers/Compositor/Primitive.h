// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRIMITIVE__
#define __RADARLOVE_COMPOSITOR_PRIMITIVE__

#include <Core/Core.h>

#include <Geometry/Geometry.h>

#include <Compositor/Frame.h>
#include <Compositor/Color.h>

namespace rl {
class Primitive {
 public:
  explicit Primitive();

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

  double opacity() const;

  void setOpacity(double opacity);

  void render(Frame& frame, const Matrix& modelViewMatrix, const Size& size);

 private:
  Color _contentColor;
  double _opacity;

  RL_DISALLOW_COPY_AND_ASSIGN(Primitive);
};
}

#endif  // __RADARLOVE_COMPOSITOR_PRIMITIVE__
