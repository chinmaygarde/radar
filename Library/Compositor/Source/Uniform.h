// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_UNIFORM_H_
#define RADAR_COMPOSITOR_UNIFORM_H_

#include "OpenGL.h"
#include <Geometry/Matrix.h>
#include <Entity/Color.h>

namespace rl {
namespace compositor {

void SetUniform(GLint index, const geom::Matrix& matrix);

void SetUniform(GLint index, const entity::Color& color);

void SetUniform(GLint index, const entity::Color& color, double alpha);

void SetUniform(GLint index, const geom::Size& size);

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_UNIFORM_H_
