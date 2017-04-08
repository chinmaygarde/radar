/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "EGLConnection.h"

namespace rl {
namespace testrunner {

class GraphicsConnection {
 public:
  GraphicsConnection();

  ~GraphicsConnection();

  bool isValid() const;

  bool activate() const;

  bool deactivate() const;

 private:
  bool _isValid;

  EGLConnection _connection;
  EGLConfig _config;
  EGLSurface _surface;
  EGLContext _context;

  RL_DISALLOW_COPY_AND_ASSIGN(GraphicsConnection);
};

}  // namespace testrunner
}  // namespace rl
