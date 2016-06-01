// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_CONTEXT_H_
#define RADAR_COMPOSITOR_CONTEXT_H_

#include <Core/Macros.h>

namespace rl {
namespace compositor {

class Context {
 public:
  Context();

  ~Context();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_CONTEXT_H_
