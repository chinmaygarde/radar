/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace core {

/**
 *  A wrapper for raw handles. Traits must be something like the following:
 *  ```
 *  struct SomeHandleTraits {
 *    static int NullHandle() { return -1; };
 *    static bool IsValid(int handle) { return handle >= 0; }
 *    static bool CollectHandle(int handle) { return true; }
 *  };
 *  ```
 */
template <class Type, class Traits>
class Handle {
 public:
  using HandleType = Type;

  Handle() : _handle(Traits::NullHandle()) {}

  explicit Handle(HandleType handle) : _handle(handle) {}

  Handle(Handle&& other) : _handle(Traits::NullHandle()) { swap(other); }

  ~Handle() { resetAndCheck(); }

  bool isValid() const { return Traits::IsValid(_handle); }

  HandleType get() const { return _handle; }

  Handle& operator=(Handle&& other) {
    if (_handle == other._handle) {
      other._handle = Traits::NullHandle();
    } else {
      swap(other);
    }
    return *this;
  }

  bool operator==(const Handle& other) const {
    return _handle == other._handle;
  }

  bool operator!=(const Handle& other) const {
    return _handle != other._handle;
  }

  void swap(Handle& other) {
    HandleType temp = _handle;
    _handle = other._handle;
    other._handle = temp;
  }

  bool reset() RL_WARN_UNUSED_RESULT {
    bool collected = false;
    if (_handle != Traits::NullHandle()) {
      collected = Traits::CollectHandle(_handle);
      _handle = Traits::NullHandle();
    }
    return collected;
  }

 private:
  HandleType _handle;

  void resetAndCheck() {
    if (_handle != Traits::NullHandle()) {
      bool collected = Traits::CollectHandle(_handle);
      RL_ASSERT(collected);
      _handle = Traits::NullHandle();
    }
  }

  RL_DISALLOW_COPY_AND_ASSIGN(Handle);
};

}  // namespace core
}  // namespace rl
