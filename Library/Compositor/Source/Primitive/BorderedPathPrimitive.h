// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_BORDEREDPATHPRIMITIVE_H_
#define RADAR_COMPOSITOR_BORDEREDPATHPRIMITIVE_H_

#include <Core/Macros.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class BorderedPathPrimitive : public Primitive {
 public:
  BorderedPathPrimitive();

  ~BorderedPathPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(BorderedPathPrimitive);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_BORDEREDPATHPRIMITIVE_H_
