// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PATCH__
#define __RADARLOVE_COMPOSITOR_PATCH__

#include <Core/Core.h>
#include <Compositor/PatchChunk.h>

#include <map>

namespace rl {
class Layer;
class Patch {
 public:
  /**
   *  Create a fresh patch for recording updates for commit to the compositor
   */
  Patch();

  /**
   *  Mark a layer property as needing update to the compositor
   *
   *  @param layer   the layer to mark dirty
   *  @param command the patch command used to resolve the update
   */
  void mark(const Layer& layer, PatchChunk::Command command);

 private:
  std::map<PatchChunk::Identifier, std::set<PatchChunk::Command>> _marks;

  DISALLOW_COPY_AND_ASSIGN(Patch);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PATCH__) */
