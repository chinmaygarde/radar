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
   *  @param chunk   the patch chunk used to resolve the update
   */
  void mark(const Layer& layer, PatchChunk chunk);

  /**
   *  Determines if this patch produces a valid actionable message.
   *
   *  @return if this patch has a valid message
   */
  bool hasMessage() const;

  /**
   *  Flatten the patch marks into a message that can be sent over a channel to
   *  the compositor
   *
   *  @return the message to be sent over the channel
   */
  Message flatten() const;

 private:
  std::map<PatchChunk::Identifier, std::set<PatchChunk::Command>> _marks;

  DISALLOW_COPY_AND_ASSIGN(Patch);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PATCH__) */
