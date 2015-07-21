// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_MATRIX__
#define __RADARLOVE_GEOMETRY_MATRIX__

#include "Geometry/Vector.h"

namespace rl {
struct Matrix {
  Vector vec[4];

  // clang-format off
  Matrix(double m0, double m1, double m2, double m3,
         double m4, double m5, double m6, double m7,
         double m8, double m9, double m10, double m11,
         double m12, double m13, double m14, double m15)
      : vec{Vector(m0, m1, m2, m3),
            Vector(m4, m5, m6, m7),
            Vector(m8, m9, m10, m11),
            Vector(m12, m13, m14, m15)} {}
  // clang-format on

  bool operator==(const Matrix& m) const {
    return vec[0] == m.vec[0] && vec[1] == m.vec[1] && vec[2] == m.vec[2] &&
           vec[3] == m.vec[3];
  }
};

// clang-format off
static const Matrix MatrixIdentity(1.0, 0.0, 0.0, 0.0,
                                   0.0, 1.0, 0.0, 0.0,
                                   0.0, 0.0, 1.0, 0.0,
                                   0.0, 0.0, 0.0, 1.0);
// clang-format on
}

#endif /* defined(__RADARLOVE_GEOMETRY_MATRIX__) */
