// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Geometry/Matrix.h"

#include <climits>

using namespace rl;

Matrix Matrix::Orthographic(double left,
                            double right,
                            double bottom,
                            double top,
                            double nearZ,
                            double farZ) {
  double ral = right + left;
  double rsl = right - left;
  double tab = top + bottom;
  double tsb = top - bottom;
  double fan = farZ + nearZ;
  double fsn = farZ - nearZ;

  // clang-format off
  return Matrix(2.0 / rsl, 0.0,         0.0,        0.0,
                0.0,       2.0 / tsb,   0.0,        0.0,
                0.0,       0.0,         -2.0 / fsn, 0.0,
                -ral / rsl, -tab / tsb, -fan / fsn, 1.0);
  // clang-format on
}

Matrix Matrix::Orthographic(const Size& size) {
  return Matrix::Orthographic(0, size.width, size.height, 0, -INT_MAX, INT_MAX);
}
