// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_
#define RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_

#include <Core/Core.h>
#include <Event/TouchEvent.h>
#include <Recognition/Variable.h>
#include <Recognition/TouchEntity.h>

namespace rl {

class ActiveTouchSet {
 public:
  ActiveTouchSet();
  ~ActiveTouchSet();

  void add(const std::vector<TouchEvent>& touches);
  void clear(const std::vector<TouchEvent>& touches);

  using PointResult = std::pair<bool /* result */, Point>;
  PointResult pointForIndex(size_t index) const;

  TouchEntity* touchEntityForProxy(Variable::Proxy proxy) const;

  size_t size() const;

 private:
  TouchEntity::IdentifierMap _activeTouches;
  std::vector<TouchEvent::Identifier> _indexedTouches;

  TouchEntity* touchEntityForIndex(size_t index) const;

  RL_DISALLOW_COPY_AND_ASSIGN(ActiveTouchSet);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_
