// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_THREADLOCAL_H_
#define RADARLOVE_CORE_THREADLOCAL_H_

#include <Core/Macros.h>

#include <pthread.h>
#include <functional>

namespace rl {
namespace core {

class ThreadLocal {
 public:
  using DestroyCallback = std::function<void(uintptr_t)>;

 private:
  class Box {
   public:
    Box(ThreadLocal::DestroyCallback destroy, uintptr_t value)
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
    ThreadLocal::DestroyCallback _destroy;
    uintptr_t _value;

    RL_DISALLOW_COPY_AND_ASSIGN(Box);
  };

  static void ThreadLocalDestroy(Box* value);

 public:
  ThreadLocal() : ThreadLocal(nullptr) {}

  ThreadLocal(DestroyCallback destroy) : _destroy(destroy) {
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
  DestroyCallback _destroy;

  RL_DISALLOW_COPY_AND_ASSIGN(ThreadLocal);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_THREADLOCAL_H_
