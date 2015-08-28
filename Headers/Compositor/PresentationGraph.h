// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__

#include <Core/Core.h>
#include <Compositor/EntityArena.h>
#include <Compositor/Frame.h>
#include <Compositor/PresentationEntity.h>
#include <Compositor/TransferRecord.h>

#include <map>

namespace rl {

class PresentationGraph {
 public:
  explicit PresentationGraph();
  ~PresentationGraph();

  void applyUpdates(Message& arena);

  void render(Frame& frame);

 private:
  std::map<Entity::Identifier, std::unique_ptr<PresentationEntity>> _entities;

  void prepareActionsAndMerge(PresentationEntity& currentState,
                              const TransferRecord& record);

  DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__
