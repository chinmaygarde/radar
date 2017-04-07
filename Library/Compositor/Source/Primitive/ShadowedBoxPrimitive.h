// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include "Primitive.h"

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
