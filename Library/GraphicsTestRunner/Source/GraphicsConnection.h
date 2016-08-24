// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GRAPHICSTESTRUNNER_GRAPHICSCONNECTION_H_
#define RADAR_GRAPHICSTESTRUNNER_GRAPHICSCONNECTION_H_

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

#endif  // RADAR_GRAPHICSTESTRUNNER_GRAPHICSCONNECTION_H_
