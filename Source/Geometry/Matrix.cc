// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Matrix.h>

#include <climits>

namespace rl {

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

Matrix Matrix::Perspective(double fov,
                           double aspect,
                           double nearZ,
                           double farZ) {
  double cotan = 1.0 / tan(fov / 2.0);

  // clang-format off
  return Matrix(cotan / aspect,
                0.0,
                0.0,
                0.0,

                0.0,
                cotan,
                0.0,
                0.0,

                0.0,
                0.0,
                (farZ + nearZ) / (nearZ - farZ),
                -1.0,

                0.0,
                0.0,
                (2.0 * farZ * nearZ) / (nearZ - farZ),
                0.0);
  // clang-format on
}

Matrix Matrix::operator+(const Matrix& o) const {
  /*
   *  FIXME: This implementation is incorrect. We need to perform a
   *  matrix decomposition on both sides and add those values to obtain the
   *  result. Once the matrix decomposition math is implemented, remove this
   *  implementation as well as the one performing the matrix interpolations
   */
  // clang-format off
  return Matrix(m[0] + o.m[0],
                m[1] + o.m[1],
                m[2] + o.m[2],
                m[3] + o.m[3],
                m[4] + o.m[4],
                m[5] + o.m[5],
                m[6] + o.m[6],
                m[7] + o.m[7],
                m[8] + o.m[8],
                m[9] + o.m[9],
                m[10] + o.m[10],
                m[11] + o.m[11],
                m[12] + o.m[12],
                m[13] + o.m[13],
                m[14] + o.m[14],
                m[15] + o.m[15]);
  // clang-format on
}

}  // namespace rl
