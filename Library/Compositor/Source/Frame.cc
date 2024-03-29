/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Context.h>
#include <Compositor/Frame.h>
#include <Core/Utilities.h>
#include <GLFoundation/GLFoundation.h>
#include "Primitive/Primitive.h"

namespace rl {
namespace compositor {

Frame::Frame(geom::Size size, Context& context)
    : _size(size),
      _projectionMatrix(geom::Matrix::Orthographic(size)),
      _context(context) {}

Frame::~Frame() = default;

bool Frame::isReady() const {
  if (_size.width <= 0.0 && _size.height <= 0.0) {
    return false;
  }

  if (glGetError() != GL_NO_ERROR) {
    return false;
  }

  return true;
}

const geom::Size& Frame::size() const {
  return _size;
}

bool Frame::begin() {
  if (!_context.beginUsing()) {
    return false;
  }

  prepareFrame();

  return true;
}

bool Frame::end() {
  _context.renderConsole(*this);

  return _context.endUsing();
}

void Frame::prepareFrame() {
  glViewport(0, 0, _size.width, _size.height);

  glScissor(0, 0, _size.width, _size.height);
  glEnable(GL_SCISSOR_TEST);

  glDisable(GL_CULL_FACE);

  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Context& Frame::context() {
  return _context;
}

const geom::Matrix& Frame::projectionMatrix() const {
  return _projectionMatrix;
}

}  // namespace compositor
}  // namespace rl
