/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <algorithm>

namespace rl {
namespace layout {

namespace priority {

static inline double Create(double a, double b, double c) {
  double result = 0.0;
  result += std::max(0.0, std::min(1e3, a) * 1e6);
  result += std::max(0.0, std::min(1e3, b) * 1e3);
  result += std::max(0.0, std::min(1e3, c));
  return result;
}

static const double Required = Create(1e3, 1e3, 1e3);
static const double Strong = Create(1.0, 0.0, 0.0);
static const double Medium = Create(0.0, 1.0, 0.0);
static const double Weak = Create(0.0, 0.0, 1.0);

}  // namespace priority

}  // namespace layout
}  // namespace rl
