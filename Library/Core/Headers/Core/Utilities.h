// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_UTILITIES_
#define RADAR_CORE_UTILITIES_

#include <Core/Timing.h>
#include <Core/URI.h>
#include <functional>
#include <memory>
#include <type_traits>

namespace rl {
namespace core {

template <bool B, class T = void>
using only_if = typename std::enable_if<B, T>::type;

#define rl_trivially_copyable(x) __has_trivial_copy(x)

template <class T>
void HashCombine(size_t& seed, T const& v) {
  seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

uint32_t NextPowerOfTwoSize(uint32_t x);

/**
 *  Converts the given clock duration to a duration in milliseconds that is
 *  suitable for passing to a unix call. Returns -1 for indefinite duration.
 *
 *  @param nano the duration to convert from
 *
 *  @return the duration in milliseconds
 */
int ToUnixTimeoutMS(ClockDurationNano nano);

URI GetExecutablePath();

URI GetResourcesPath();

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_UTILITIES_
