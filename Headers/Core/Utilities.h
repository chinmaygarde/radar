// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_UTILITIES_
#define RADARLOVE_CORE_UTILITIES_

#include <Core/Timing.h>

#include <functional>
#include <memory>
#include <type_traits>

namespace rl {
namespace core {

/**
 *  std::make_unique replacement for C++11
 *
 *  @param args the arguments to make_unique (forwarded)
 *
 *  @return the unique pointer
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <bool B, class T = void>
using only_if = typename std::enable_if<B, T>::type;

#define rl_trivially_copyable(x) __has_trivial_copy(x)

template <typename T>
void HashCombine(size_t& seed, T const& v) {
  seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

/**
 *  Converts the given clock duration to a duration in milliseconds that is
 *  suitable for passing to a unix call. Returns -1 for indefinite duration.
 *
 *  @param nano the duration to convert from
 *
 *  @return the duration in milliseconds
 */
int ToUnixTimeoutMS(ClockDurationNano nano);

/**
 *  Tries to log various properties about the UNIX handle (used for debugging)
 *
 *  @param handle the handle to log properties on
 */
void LogUnixHandleProperties(int handle);

namespace thread {
/**
 *  Set the name of current thread of execution
 *
 *  @param name the name to set
 */
void SetName(const char* name);

/**
 *  Get the name of the current thread of execution
 *
 *  @return the name of the thread
 */
std::string GetName();

}  // namespace thread
}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_UTILITIES_
