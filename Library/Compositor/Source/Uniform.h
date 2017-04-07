// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Entity/Color.h>
#include <Geometry/Matrix.h>
#include "OpenGL.h"

namespace rl {
namespace compositor {

void SetUniform(GLint index, const geom::Matrix& matrix);

void SetUniform(GLint index, const entity::Color& color);

void SetUniform(GLint index, const entity::Color& color, double alpha);

void SetUniform(GLint index, double alpha);

void SetUniform(GLint index, const geom::Size& size);

}  // namespace compositor
}  // namespace rl
