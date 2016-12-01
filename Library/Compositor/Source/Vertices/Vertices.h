// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_VERTICES_H_
#define RADAR_COMPOSITOR_VERTICES_H_

#include <Core/Macros.h>
#include <Core/ThreadGuard.h>
#include "OpenGL.h"

namespace rl {
namespace compositor {

class Vertices {
 public:
  enum class Type {
    Array,
    ElementArray,
  };

  Vertices(Type type);

  virtual ~Vertices();

  bool prepare();

  bool draw(size_t index);

 private:
  RL_DEBUG_THREAD_GUARD(_guard);
  const Type _type;
  ResourceState _state;
  GLuint _vbo;
  GLuint _ibo;

  bool prepareBuffers();
  bool use();
  virtual bool doDraw(size_t index) = 0;
  virtual bool uploadVertexData() = 0;

  RL_DISALLOW_COPY_AND_ASSIGN(Vertices);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_VERTICES_H_
