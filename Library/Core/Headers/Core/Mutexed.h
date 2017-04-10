/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/Mutex.h>

namespace rl {
namespace core {

template <class Resource, class BasicLockable = Mutex>
class Mutexed {
 public:
  using ResourceType = Resource;
  using LockType = BasicLockable;

  class Access {
   public:
    Access(Access&&) = default;

    ~Access() { _mutex.Unlock(); }

    Resource& get() { return _resource; }

   private:
    BasicLockable& _mutex;
    Resource& _resource;

    friend class Mutexed;

    Access(BasicLockable& mutex, Resource& resource)
        : _mutex(mutex), _resource(resource) {
      _mutex.Lock();
    }

    RL_DISALLOW_COPY_AND_ASSIGN(Access);
  };

  template <typename... Args>
  Mutexed(Args&&... args) : _resource(std::forward<Args>(args)...) {}

  Access access() { return Access{_mutex, _resource}; }

 private:
  BasicLockable _mutex;
  Resource _resource RL_GUARDED_BY(_mutex);

  RL_DISALLOW_COPY_AND_ASSIGN(Mutexed);
};

}  // namespace core
}  // namespace rl
