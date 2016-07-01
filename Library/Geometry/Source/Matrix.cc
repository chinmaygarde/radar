// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Matrix.h>

#include <climits>
#include <sstream>

namespace rl {
namespace geom {

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
  return Matrix(2.0 / rsl,   0.0,          0.0,        0.0,
                0.0,         2.0 / tsb,    0.0,        0.0,
                0.0,         0.0,         -2.0 / fsn,  0.0,
                -ral / rsl, -tab / tsb,   -fan / fsn,  1.0);
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

  return Matrix(cotan / aspect, 0.0, 0.0, 0.0,                        //
                0.0, cotan, 0.0, 0.0,                                 //
                0.0, 0.0, (farZ + nearZ) / (nearZ - farZ), -1.0,      //
                0.0, 0.0, (2.0 * farZ * nearZ) / (nearZ - farZ), 0.0  //
                );
}

Matrix Matrix::LookAt(const Vector3& eye,
                      const Vector3& center,
                      const Vector3& up) {
  auto n = (eye - center).normalize();
  auto u = (up.cross(n)).normalize();
  auto v = n.cross(u);
  return {u.x,           v.x,           n.x,           0.0,  //
          u.y,           v.y,           n.y,           0.0,  //
          u.z,           v.z,           n.z,           0.0,  //
          (-u).dot(eye), (-v).dot(eye), (-n).dot(eye), 1.0};
}

Matrix Matrix::operator+(const Matrix& o) const {
  return Matrix(
      m[0] + o.m[0], m[1] + o.m[1], m[2] + o.m[2], m[3] + o.m[3],         //
      m[4] + o.m[4], m[5] + o.m[5], m[6] + o.m[6], m[7] + o.m[7],         //
      m[8] + o.m[8], m[9] + o.m[9], m[10] + o.m[10], m[11] + o.m[11],     //
      m[12] + o.m[12], m[13] + o.m[13], m[14] + o.m[14], m[15] + o.m[15]  //
      );
}

std::string Matrix::toString() const {
  std::stringstream stream;
  for (int i = 0, limit = 16; i < limit; i++) {
    stream << m[i];
    if (i != limit - 1) {
      stream << ",";
    }
  }
  return stream.str();
}

void Matrix::fromString(const std::string& str) {
  std::stringstream stream(str);
  for (int i = 0; i < 16; i++) {
    stream >> m[i];
    stream.ignore();
  }
}

Matrix Matrix::invert() const {
  Matrix tmp{
      m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
          m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10],

      -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] -
          m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10],

      m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] +
          m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6],

      -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] -
          m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6],

      -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] -
          m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10],

      m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] +
          m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10],

      -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] -
          m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6],

      m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] +
          m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6],

      m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] +
          m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9],

      -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] -
          m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9],

      m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] +
          m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5],

      -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -
          m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5],

      -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] -
          m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9],

      m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] +
          m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9],

      -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] -
          m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5],

      m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] +
          m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5]};

  double det =
      m[0] * tmp.m[0] + m[1] * tmp.m[4] + m[2] * tmp.m[8] + m[3] * tmp.m[12];

  if (det == 0) {
    return {};
  }

  det = 1.0 / det;

  return {tmp.m[0] * det,  tmp.m[1] * det,  tmp.m[2] * det,  tmp.m[3] * det,
          tmp.m[4] * det,  tmp.m[5] * det,  tmp.m[6] * det,  tmp.m[7] * det,
          tmp.m[8] * det,  tmp.m[9] * det,  tmp.m[10] * det, tmp.m[11] * det,
          tmp.m[12] * det, tmp.m[13] * det, tmp.m[14] * det, tmp.m[15] * det};
}

Matrix Matrix::transpose() const {
  return {
      m[0], m[4], m[8],  m[12],  //
      m[1], m[5], m[9],  m[13],  //
      m[2], m[6], m[10], m[14],  //
      m[3], m[7], m[11], m[15],  //
  };
}

double Matrix::determinant() const {
  auto a00 = e[0][0];
  auto a01 = e[0][1];
  auto a02 = e[0][2];
  auto a03 = e[0][3];
  auto a10 = e[1][0];
  auto a11 = e[1][1];
  auto a12 = e[1][2];
  auto a13 = e[1][3];
  auto a20 = e[2][0];
  auto a21 = e[2][1];
  auto a22 = e[2][2];
  auto a23 = e[2][3];
  auto a30 = e[3][0];
  auto a31 = e[3][1];
  auto a32 = e[3][2];
  auto a33 = e[3][3];

  auto b00 = a00 * a11 - a01 * a10;
  auto b01 = a00 * a12 - a02 * a10;
  auto b02 = a00 * a13 - a03 * a10;
  auto b03 = a01 * a12 - a02 * a11;
  auto b04 = a01 * a13 - a03 * a11;
  auto b05 = a02 * a13 - a03 * a12;
  auto b06 = a20 * a31 - a21 * a30;
  auto b07 = a20 * a32 - a22 * a30;
  auto b08 = a20 * a33 - a23 * a30;
  auto b09 = a21 * a32 - a22 * a31;
  auto b10 = a21 * a33 - a23 * a31;
  auto b11 = a22 * a33 - a23 * a32;

  return b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
}

/*
 *  Adapted for Radar from Graphics Gems:
 *  http://www.realtimerendering.com/resources/GraphicsGems/gemsii/unmatrix.c
 */
Matrix::DecompositionResult Matrix::decompose() const {
  /*
   *  Normalize the matrix.
   */
  Matrix locmat = *this;

  if (locmat.e[3][3] == 0) {
    return {false, {}};
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      locmat.e[i][j] /= locmat.e[3][3];
    }
  }

  /*
   *  pmat is used to solve for perspective, but it also provides
   *  an easy way to test for singularity of the upper 3x3 component.
   */
  Matrix pmat = locmat;
  for (int i = 0; i < 3; i++) {
    pmat.e[i][3] = 0;
  }

  pmat.e[3][3] = 1;

  if (pmat.determinant() == 0.0) {
    return {false, {}};
  }

  Decomposition res;

  /*
   *  ==========================================================================
   *  First, isolate perspective.
   *  ==========================================================================
   */
  if (locmat.e[0][3] != 0.0 || locmat.e[1][3] != 0.0 || locmat.e[2][3] != 0.0) {
    /*
     *  prhs is the right hand side of the equation.
     */
    const Vector4 prhs(locmat.e[0][3],  //
                       locmat.e[1][3],  //
                       locmat.e[2][3],  //
                       locmat.e[3][3]);

    /*
     *  Solve the equation by inverting pmat and multiplying
     *  prhs by the inverse.
     */

    res.perspective = prhs * pmat.invert().transpose();

    /*
     *  Clear the perspective partition.
     */
    locmat.e[0][3] = locmat.e[1][3] = locmat.e[2][3] = 0;
    locmat.e[3][3] = 1;
  }

  /*
   *  ==========================================================================
   *  Next, the translation.
   *  ==========================================================================
   */
  res.translation = {locmat.e[3][0], locmat.e[3][1], locmat.e[3][2]};
  locmat.e[3][0] = locmat.e[3][1] = locmat.e[3][2] = 0.0;

  /*
   *  ==========================================================================
   *  Next, the scale and shear.
   *  ==========================================================================
   */
  Vector3 row[3];
  for (int i = 0; i < 3; i++) {
    row[i].x = locmat.e[i][0];
    row[i].y = locmat.e[i][1];
    row[i].z = locmat.e[i][2];
  }

  /*
   *  Compute X scale factor and normalize first row.
   */
  res.scale.x = row[0].length();
  row[0] = row[0].scale(1.0);

  /*
   *  Compute XY shear factor and make 2nd row orthogonal to 1st.
   */
  res.shear.xy = row[0].dot(row[1]);
  row[1] = Vector3::Combine(row[1], 1.0, row[0], -res.shear.xy);

  /*
   *  Compute Y scale and normalize 2nd row.
   */
  res.scale.y = row[1].length();
  row[1] = row[1].scale(1.0);
  res.shear.xy /= res.scale.y;

  /*
   *  Compute XZ and YZ shears, orthogonalize 3rd row.
   */
  res.shear.xz = row[0].dot(row[2]);
  row[2] = Vector3::Combine(row[2], 1.0, row[0], -res.shear.xz);
  res.shear.yz = row[1].dot(row[2]);
  row[2] = Vector3::Combine(row[2], 1.0, row[1], -res.shear.yz);

  /*
   *  Next, get Z scale and normalize 3rd row.
   */
  res.scale.z = row[2].length();
  row[2] = row[2].scale(1.0);

  res.shear.xz /= res.scale.z;
  res.shear.yz /= res.scale.z;

  /*
   *  At this point, the matrix (in rows[]) is orthonormal.
   *  Check for a coordinate system flip.  If the determinant
   *  is -1, then negate the matrix and the scaling factors.
   */
  if (row[0].dot(row[1].cross(row[2])) < 0) {
    res.scale.x *= -1;
    res.scale.y *= -1;
    res.scale.z *= -1;

    for (int i = 0; i < 3; i++) {
      row[i].x *= -1;
      row[i].y *= -1;
      row[i].z *= -1;
    }
  }

  /*
   *  Get the rotations out.
   */
  res.rotation.y = asin(-row[0].z);
  if (cos(res.rotation.y) != 0) {
    res.rotation.x = atan2(row[1].z, row[2].z);
    res.rotation.z = atan2(row[0].y, row[0].x);
  } else {
    res.rotation.x = atan2(-row[2].x, row[1].y);
    res.rotation.z = 0.0;
  }

  return DecompositionResult(true, res);
}

}  // namespace geom
}  // namespace rl
