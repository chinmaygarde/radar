// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MUTEXED_H_
#define RADAR_CORE_MUTEXED_H_

#include <Core/Macros.h>

namespace rl {
namespace core {

template <class Resource, class BasicLockable>
class Mutexed {
 public:
  class Access {
   public:
    Access(Access&&) = default;

    ~Access() { _mutex.unlock(); }

    Resource& get() { return _resource; }

   private:
    BasicLockable& _mutex;
    Resource& _resource;

    friend class Mutexed;

    Access(BasicLockable& mutex, Resource& resource)
        : _mutex(mutex), _resource(resource) {
      _mutex.lock();
    }

    RL_DISALLOW_COPY_AND_ASSIGN(Access);
  };

  template <typename... Args>
  Mutexed(Args&&... args) : _resource(std::forward<Args>(args)...) {}

  Access access() { return Access{_mutex, _resource}; }

 private:
  BasicLockable _mutex;
  Resource _resource;

  RL_DISALLOW_COPY_AND_ASSIGN(Mutexed);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_MUTEXED_H_
