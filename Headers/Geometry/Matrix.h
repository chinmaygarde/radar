// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_MATRIX__
#define __RADARLOVE_GEOMETRY_MATRIX__

#include "Geometry/Vector.h"
#include "Geometry/Size.h"
#include "Geometry/Point.h"

#include <cmath>

namespace rl {
struct Matrix {
  union {
    double m[16];
    Vector4 vec[4];
  };

  // clang-format off
  Matrix(double m0,  double m1,  double m2,  double m3,
         double m4,  double m5,  double m6,  double m7,
         double m8,  double m9,  double m10, double m11,
         double m12, double m13, double m14, double m15)
      : vec{Vector4(m0,  m1,  m2,  m3),
            Vector4(m4,  m5,  m6,  m7),
            Vector4(m8,  m9,  m10, m11),
            Vector4(m12, m13, m14, m15)} {}
  // clang-format on

  static Matrix Translation(const Vector3& t) {
    // clang-format off
    return Matrix(1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  t.x, t.y, t.z, 1.0);
    // clang-format on
  }

  static Matrix Scale(const Vector3& s) {
    // clang-format off
    return Matrix(s.x, 0.0, 0.0, 0.0,
                  0.0, s.y, 0.0, 0.0,
                  0.0, 0.0, s.z, 0.0,
                  0.0, 0.0, 0.0, 1.0);
    // clang-format on
  }

  static Matrix Rotation(double radians, const Vector4& r) {
    const Vector4 v = r.normalize();

    const double cosine = cos(radians);
    const double cosp = 1.0f - cosine;
    const double sine = sin(radians);

    // clang-format off
    return Matrix(
      cosine + cosp * v.a * v.a,
      cosp * v.a * v.b + v.c * sine,
      cosp * v.a * v.c - v.b * sine,
      0.0,

      cosp * v.a * v.b - v.c * sine,
      cosine + cosp * v.b * v.b,
      cosp * v.b * v.c + v.a * sine,
      0.0,

      cosp * v.a * v.c + v.b * sine,
      cosp * v.b * v.c - v.a * sine,
      cosine + cosp * v.c * v.c,
      0.0,

      0.0,
      0.0,
      0.0,
      1.0);
    // clang-format on
  }

  static Matrix RotationX(double radians) {
    double cosine = cos(radians);
    double sine = sin(radians);
    // clang-format off
    return Matrix(
      1.0,  0.0,    0.0,    0.0,
      0.0,  cosine, sine,   0.0,
      0.0, -sine,   cosine, 0.0,
      0.0,  0.0,    0.0,    1.0
    );
    // clang-format on
  }

  static Matrix RotationY(double radians) {
    double cosine = cos(radians);
    double sine = sin(radians);

    // clang-format off
    return Matrix(
      cosine, 0.0, -sine,   0.0,
      0.0,    1.0,  0.0,    0.0,
      sine,   0.0,  cosine, 0.0,
      0.0,    0.0,  0.0,    1.0
    );
    // clang-format on
  }

  static Matrix RotationZ(double radians) {
    double cosine = cos(radians);
    double sine = sin(radians);

    // clang-format off
    return Matrix (
      cosine, sine,   0.0, 0.0,
      -sine,  cosine, 0.0, 0.0,
      0.0,    0.0,    1.0, 0.0,
      0.0,    0.0,    0.0, 1.0
    );
    // clang-format on
  }

  Matrix translate(const Vector3& t) const {
    // clang-format off
    return Matrix(m[0], m[1], m[2], m[3],
                  m[4], m[5], m[6], m[7],
                  m[8], m[9], m[10], m[11],
                  m[0] * t.x + m[4] * t.y + m[8] * t.z + m[12],
                  m[1] * t.x + m[5] * t.y + m[9] * t.z + m[13],
                  m[2] * t.x + m[6] * t.y + m[10] * t.z + m[14],
                  m[15]);
    // clang-format on
  }

  Matrix scale(const Vector3& s) const {
    // clang-format off
    return Matrix(m[0] * s.x, m[1] * s.x, m[2] * s.x , m[3] * s.x,
                  m[4] * s.y, m[5] * s.y, m[6] * s.y , m[7] * s.y,
                  m[8] * s.z, m[9] * s.z, m[10] * s.z, m[11] * s.z,
                  m[12]     , m[13]     , m[14]      , m[15]);
    // clang-format on
  }

  Matrix multiply(const Matrix& o) const {
    // clang-format off
    return Matrix(
        m[0] * o.m[0]  + m[4] * o.m[1]  + m[8]  * o.m[2]  + m[12] * o.m[3],
        m[1] * o.m[0]  + m[5] * o.m[1]  + m[9]  * o.m[2]  + m[13] * o.m[3],
        m[2] * o.m[0]  + m[6] * o.m[1]  + m[10] * o.m[2]  + m[14] * o.m[3],
        m[3] * o.m[0]  + m[7] * o.m[1]  + m[11] * o.m[2]  + m[15] * o.m[3],
        m[0] * o.m[4]  + m[4] * o.m[5]  + m[8]  * o.m[6]  + m[12] * o.m[7],
        m[1] * o.m[4]  + m[5] * o.m[5]  + m[9]  * o.m[6]  + m[13] * o.m[7],
        m[2] * o.m[4]  + m[6] * o.m[5]  + m[10] * o.m[6]  + m[14] * o.m[7],
        m[3] * o.m[4]  + m[7] * o.m[5]  + m[11] * o.m[6]  + m[15] * o.m[7],
        m[0] * o.m[8]  + m[4] * o.m[9]  + m[8]  * o.m[10] + m[12] * o.m[11],
        m[1] * o.m[8]  + m[5] * o.m[9]  + m[9]  * o.m[10] + m[13] * o.m[11],
        m[2] * o.m[8]  + m[6] * o.m[9]  + m[10] * o.m[10] + m[14] * o.m[11],
        m[3] * o.m[8]  + m[7] * o.m[9]  + m[11] * o.m[10] + m[15] * o.m[11],
        m[0] * o.m[12] + m[4] * o.m[13] + m[8]  * o.m[14] + m[12] * o.m[15],
        m[1] * o.m[12] + m[5] * o.m[13] + m[9]  * o.m[14] + m[13] * o.m[15],
        m[2] * o.m[12] + m[6] * o.m[13] + m[10] * o.m[14] + m[14] * o.m[15],
        m[3] * o.m[12] + m[7] * o.m[13] + m[11] * o.m[14] + m[15] * o.m[15]);
    // clang-format on
  }

  bool operator==(const Matrix& m) const {
    // clang-format off
    return vec[0] == m.vec[0]
        && vec[1] == m.vec[1]
        && vec[2] == m.vec[2]
        && vec[3] == m.vec[3];
    // clang-format on
  }

  bool operator!=(const Matrix& m) const {
    // clang-format off
    return vec[0] != m.vec[0]
        || vec[1] != m.vec[1]
        || vec[2] != m.vec[2]
        || vec[3] != m.vec[3];
    // clang-format on
  }

  Matrix operator+(const Vector3& t) const { return translate(t); }

  Matrix operator-(const Vector3& t) const { return translate(-t); }

  Matrix operator*(const Vector3& s) const { return scale(s); };

  Matrix operator*(const Matrix& m) const { return multiply(m); };

  // clang-format off
  static Matrix Orthographic(double left,   double right,
                             double bottom, double top,
                             double nearZ,  double farZ);
  // clang-format on

  static Matrix Orthographic(const Size& size);

  // clang-format off
  static Matrix Perspective(double fov,   double aspect,
                            double nearZ, double farZ);
  // clang-format on
};

// clang-format off
static const Matrix MatrixIdentity(1.0, 0.0, 0.0, 0.0,
                                   0.0, 1.0, 0.0, 0.0,
                                   0.0, 0.0, 1.0, 0.0,
                                   0.0, 0.0, 0.0, 1.0);
// clang-format on
}

#endif /* defined(__RADARLOVE_GEOMETRY_MATRIX__) */
