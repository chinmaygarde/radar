// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_VERTICES_H_
#define RADAR_COMPOSITOR_VERTICES_H_

#include <Core/Macros.h>

#include "OpenGL.h"

namespace rl {
namespace compositor {

class Vertices {
 public:
  Vertices();

  virtual ~Vertices();

  bool prepare();

  virtual bool draw(size_t index) = 0;

  bool dispose();

 protected:
  virtual bool uploadVertexData() = 0;
  virtual bool use() = 0;

  GLuint vbo() const;

 private:
  bool _prepared;
  bool _disposed;

  GLuint _vbo;

  RL_DISALLOW_COPY_AND_ASSIGN(Vertices);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_VERTICES_H_
