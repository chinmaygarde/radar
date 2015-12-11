// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_
#define RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_

#include <Core/Core.h>
#include <Event/TouchEvent.h>
#include <Layout/Constraint.h>
#include <Recognition/TouchEntity.h>
#include <Recognition/Variable.h>

namespace rl {
namespace recognition {

class ActiveTouchSet {
 public:
  ActiveTouchSet();

  void registerProxyConstraint(layout::Constraint&& constraint);

  void applyTouchMap(const event::TouchEvent::PhaseMap& map);

 private:
  TouchEntity::IdentifierMap _activeTouches;
  std::vector<event::TouchEvent::Identifier> _indexedTouches;
  std::vector<layout::Constraint> _proxiedConstraints;

  void addTouches(const std::vector<event::TouchEvent>& touches);
  void updateTouches(const std::vector<event::TouchEvent>& touches);
  void clearTouches(const std::vector<event::TouchEvent>& touches);

  using PointResult = std::pair<bool /* result */, geom::Point>;
  PointResult pointForIndex(size_t index) const;

  TouchEntity* touchEntityForProxy(Variable::Proxy proxy) const;

  TouchEntity* touchEntityForIndex(size_t index) const;

  size_t size() const;

  RL_DISALLOW_COPY_AND_ASSIGN(ActiveTouchSet);
};

}  // namespace recognition
}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_
