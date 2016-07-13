// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_SHADOWEDPATHPRIMITIVE_H_
#define RADAR_COMPOSITOR_SHADOWEDPATHPRIMITIVE_H_

#include <Core/Macros.h>
#include <Compositor/Primitive/Primitive.h>

namespace rl {
namespace compositor {

class ShadowedPathPrimitive : public Primitive {
 public:
  ShadowedPathPrimitive();

  ~ShadowedPathPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ShadowedPathPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_SHADOWEDPATHPRIMITIVE_H_