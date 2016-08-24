// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GraphicsConnection.h"

namespace rl {
namespace testrunner {

GraphicsConnection::GraphicsConnection() : _isValid(false) {
  if (!_connection.isValid()) {
    return;
  }

  const auto type(EGLSurface::Type::PBuffer);
  const EGLConfig::BufferComponents components(8, 8, 8, 8, 0, 0);

  _config = _connection.createConfig(type, components);

  if (!_config.isValid()) {
    return;
  }

  _surface = _connection.createSurface(type, _config, {1, 1});

  if (!_surface.isValid()) {
    return;
  }

  _context = _connection.createContext(_config);

  if (!_context.isValid()) {
    return;
  }

  _isValid = true;
}

GraphicsConnection::~GraphicsConnection() = default;

bool GraphicsConnection::isValid() const {
  return _isValid;
}

bool GraphicsConnection::activate() const {
  return _context.makeCurrent(_surface);
}

bool GraphicsConnection::deactivate() const {
  return _context.clearCurrent();
}

}  // namespace testrunner
}  // namespace rl
