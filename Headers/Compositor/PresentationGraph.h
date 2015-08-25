// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__

#include <Core/Core.h>
#include <Compositor/EntityArena.h>

namespace rl {

class PresentationGraph {
 public:
  PresentationGraph();
  ~PresentationGraph();

  void applyUpdates(EntityArena& arena);

 private:
  DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__
