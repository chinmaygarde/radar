// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_SHADOWEDBOXPRIMITIVE_H_
#define RADAR_COMPOSITOR_SHADOWEDBOXPRIMITIVE_H_

#include <Core/Macros.h>
#include <Compositor/Primitive/Primitive.h>

namespace rl {
namespace compositor {

class ShadowedBoxPrimitive : public Primitive {
 public:
  ShadowedBoxPrimitive();

  ~ShadowedBoxPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ShadowedBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_SHADOWEDBOXPRIMITIVE_H_
