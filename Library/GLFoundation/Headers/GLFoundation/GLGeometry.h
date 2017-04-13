/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <GLFoundation/OpenGL.h>
#include <Geometry/Matrix.h>

namespace rl {
namespace gl {

enum class GLResourceState {
  NotReady,
  Failed,
  Ready,
};

struct GLPoint {
  GLfloat x;
  GLfloat y;

  GLPoint(double x1, double y1) : x(x1), y(y1) {}

  GLPoint(const geom::Point& p) : x(p.x), y(p.y) {}
};

struct GLMatrix {
  GLfloat m[16];

  GLMatrix(const geom::Matrix& o)
      : m{
            static_cast<GLfloat>(o.m[0]),  static_cast<GLfloat>(o.m[1]),
            static_cast<GLfloat>(o.m[2]),  static_cast<GLfloat>(o.m[3]),
            static_cast<GLfloat>(o.m[4]),  static_cast<GLfloat>(o.m[5]),
            static_cast<GLfloat>(o.m[6]),  static_cast<GLfloat>(o.m[7]),
            static_cast<GLfloat>(o.m[8]),  static_cast<GLfloat>(o.m[9]),
            static_cast<GLfloat>(o.m[10]), static_cast<GLfloat>(o.m[11]),
            static_cast<GLfloat>(o.m[12]), static_cast<GLfloat>(o.m[13]),
            static_cast<GLfloat>(o.m[14]), static_cast<GLfloat>(o.m[15]),
        } {};
};

}  // namespace gl
}  // namespace rl

static_assert(sizeof(rl::gl::GLMatrix) == 16 * sizeof(GLfloat),
              "GLMatrix must have the expected size");
