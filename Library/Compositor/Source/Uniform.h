/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Entity/Color.h>
#include <GLFoundation/GLFoundation.h>
#include <Geometry/Matrix.h>

namespace rl {
namespace compositor {

void SetUniform(GLint index, const geom::Matrix& matrix);

void SetUniform(GLint index, const entity::Color& color);

void SetUniform(GLint index, const entity::Color& color, double alpha);

void SetUniform(GLint index, double alpha);

void SetUniform(GLint index, const geom::Size& size);

}  // namespace compositor
}  // namespace rl
