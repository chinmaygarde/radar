/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
