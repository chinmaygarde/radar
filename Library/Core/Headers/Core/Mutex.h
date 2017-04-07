// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MUTEX_H_
#define RADAR_CORE_MUTEX_H_

#include <Core/Macros.h>
#include <Core/ThreadSafetyAnalysis.h>
#include <thread>

namespace rl {
namespace core {

/*
 *  Defines an annotated interface for mutexes.
 *  These methods can be implemented to use any internal mutex implementation.
 */
class RL_CAPABILITY("mutex") Mutex {
 public:
  Mutex() = default;

  ~Mutex() = default;

  void Lock() RL_ACQUIRE() { _mutex.lock(); }

  void Unlock() RL_RELEASE() { _mutex.unlock(); }

 private:
  std::mutex _mutex;

  RL_DISALLOW_COPY_AND_ASSIGN(Mutex);
};

/*
 *  MutexLocker is an RAII class that acquires a mutex in its constructor, and
 *  releases it in its destructor.
 */
class RL_SCOPED_CAPABILITY MutexLocker {
 public:
  MutexLocker(Mutex& mutex) RL_ACQUIRE(mutex) : _mutex(mutex) { _mutex.Lock(); }

  ~MutexLocker() RL_RELEASE() { _mutex.Unlock(); }

 private:
  Mutex& _mutex;

  RL_DISALLOW_COPY_AND_ASSIGN(MutexLocker);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_MUTEX_H_
