// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Namespace.h>

namespace rl {
namespace core {

Namespace::Namespace() : _last(DeadName) {}

Name Namespace::create(Name counterpart) {
  if (counterpart == DeadName) {
    return ++_last;
  }

  std::lock_guard<std::mutex> lock(_lock);

  /*
   *  Try to emplace a fresh local name into the counterpart keyed map
   */
  auto newName = ++_last;
  auto result = _counterpartToLocalMap.emplace(counterpart, newName);

  if (!result.second) {
    /*
     *  If the emplace fails, we already know of a live local key. Since we
     *  already have an iterator to it, return it now
     */
    RL_ASSERT((*(result.first)).second == counterpart);
    return (*(result.first)).second;
  } else {
    /*
     *  We were able to emplace a name into the counterparts map, emplace the
     *  same into the locals map for lookup during destruction.
     */
    auto result = _localToCounterpartMap.emplace(newName, counterpart);
    RL_ASSERT(result.second /* since its a fresh local key */);
    return newName;
  }

  return DeadName;
}

void Namespace::destroy(Name local) {
  if (local == DeadName) {
    return;
  }

  std::lock_guard<std::mutex> lock(_lock);

  auto found = _localToCounterpartMap.find(local);

  if (found == _localToCounterpartMap.end()) {
    /*
     *  If no entry was found in the local to counterparts map, this name was
     *  purely local. Nothing more to do.
     */
    return;
  }

  auto erased = _counterpartToLocalMap.erase(found->second);
  RL_ASSERT(erased == 1);
  _localToCounterpartMap.erase(found);
}

}  // namespace core
}  // namespace rl
