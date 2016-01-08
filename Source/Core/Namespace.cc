// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Namespace.h>

#include <Core/Message.h>

namespace rl {
namespace core {

Name::Name() : _handle(DeadHandle), _ns(nullptr) {}

Name::Name(Namespace* ns)
    : _handle(ns == nullptr ? DeadHandle : ns->createHandle()), _ns(ns) {}

Name::Name(Handle handle, Namespace* ns) : _handle(handle), _ns(ns) {}

Name::~Name() {
  if (_ns) {
    _ns->destroy(_handle);
  }
}

bool Name::serialize(Message& message) const {
  return message.encode(_handle);
}

bool Name::deserialize(Message& message) {
  return message.decode(_handle);
}

Namespace* Name::ns() const {
  return _ns;
}

Name::Handle Name::handle() const {
  return _handle;
}

size_t Name::hash() const {
  return std::hash<Handle>()(_handle);
}

Namespace::Namespace() : _last(DeadHandle) {}

Name::Handle Namespace::createHandle(Name::Handle counterpart) {
  if (counterpart == DeadHandle) {
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

  return DeadHandle;
}

Name Namespace::create(Name::Handle counterpart) {
  return {createHandle(counterpart), this};
}

void Namespace::destroy(Name::Handle local) {
  if (local == DeadHandle) {
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

const Name DeadName(DeadHandle, nullptr);

}  // namespace core
}  // namespace rl
