// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH__

#include <Core/Core.h>
#include <Compositor/PatchChunk.h>
#include <Compositor/PresentationLayer.h>

#include <map>

namespace rl {
class PresentationGraph {
 public:
  /**
   *  Create a mirror of the graph on the interface. This graph is updated when
   *  the interface sends patch messages to this instance. The layers created
   *  as part of this graph are presentation layers.
   */
  PresentationGraph();

  /**
   *  Apply a patch to this graph with data sent by the interface
   *
   *  @param messages the patch messages
   *
   *  @return if the patches were successfully applied.
   */
  bool applyPatches(Messages messages);

 private:
  std::map<PatchChunk::Identifier, PresentationLayer::Ref> _layers;

  bool applyPatchSingle(Message& message);
  bool applyChunk(const PatchChunk& chunk);

  DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH__) */
