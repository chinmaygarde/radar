// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Platform.h>

#define RL_THREAD_LOCAL_PTHREADS \
  (RL_OS_NACL || RL_OS_MAC || RL_OS_ANDROID || RL_OS_BSD || RL_OS_RASPBERRYPI)

#include <Core/Macros.h>
#include <functional>

#if RL_THREAD_LOCAL_PTHREADS
#include <pthread.h>
#endif

namespace rl {
namespace core {

using ThreadLocalDestroyCallback = std::function<void(uintptr_t)>;

#if RL_THREAD_LOCAL_PTHREADS

#define RL_THREAD_LOCAL static

class ThreadLocal {
 private:
  class Box {
   public:
    Box(ThreadLocalDestroyCallback destroy, uintptr_t value)
        : _destroy(destroy), _value(value) {}

    uintptr_t value() const { return _value; }

    void setValue(uintptr_t value) {
      if (value == _value) {
        return;
      }

      destroyValue();
      _value = value;
    }

    void destroyValue() {
      if (_destroy) {
        _destroy(_value);
      }
    }

   private:
    ThreadLocalDestroyCallback _destroy;
    uintptr_t _value;

    RL_DISALLOW_COPY_AND_ASSIGN(Box);
  };

  static void ThreadLocalDestroy(Box* value);

 public:
  ThreadLocal() : ThreadLocal(nullptr) {}

  ThreadLocal(ThreadLocalDestroyCallback destroy) : _destroy(destroy) {
    auto callback = reinterpret_cast<void (*)(void*)>(&ThreadLocalDestroy);
    RL_CHECK(pthread_key_create(&_key, callback));
  }

  void set(uintptr_t value) {
    auto box = reinterpret_cast<Box*>(pthread_getspecific(_key));
    if (box == nullptr) {
      box = new Box(_destroy, value);
      RL_CHECK(pthread_setspecific(_key, box));
    } else {
      box->setValue(value);
    }
  }

  uintptr_t get() {
    auto box = reinterpret_cast<Box*>(pthread_getspecific(_key));
    return box != nullptr ? box->value() : 0;
  }

  ~ThreadLocal() {
    /*
     *  This will NOT call the destroy callbacks on thread local values still
     *  active in other threads. Those must be cleared manually. The usage
     *  of this class should be similar to the thread_local keyword but with
     *  with a static storage specifier
     */

    /*
     *  Collect the container
     */
    delete reinterpret_cast<Box*>(pthread_getspecific(_key));

    /*
     *  Finally, collect the key
     */
    RL_CHECK(pthread_key_delete(_key));
  }

 private:
  pthread_key_t _key;
  ThreadLocalDestroyCallback _destroy;

  RL_DISALLOW_COPY_AND_ASSIGN(ThreadLocal);
};

#else  // RL_THREAD_LOCAL_PTHREADS

#define RL_THREAD_LOCAL thread_local

class ThreadLocal {
 public:
  ThreadLocal() : ThreadLocal(nullptr) {}

  ThreadLocal(ThreadLocalDestroyCallback destroy)
      : _destroy(destroy), _value(0) {}

  void set(uintptr_t value) {
    if (_value == value) {
      return;
    }

    if (_value != 0 && _destroy) {
      _destroy(_value);
    }

    _value = value;
  }

  uintptr_t get() { return _value; }

  ~ThreadLocal() {
    if (_value != 0 && _destroy) {
      _destroy(_value);
    }
  }

 private:
  ThreadLocalDestroyCallback _destroy;
  uintptr_t _value;

  RL_DISALLOW_COPY_AND_ASSIGN(ThreadLocal);
};

#endif  // RL_THREAD_LOCAL_PTHREADS

#ifndef RL_THREAD_LOCAL

#error Thread local storage unavailable on the platform.

#endif

}  // namespace core
}  // namespace rl
