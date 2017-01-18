// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_LAYOUT_PRIORITY_H_
#define RADAR_LAYOUT_PRIORITY_H_

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

#endif  // RADAR_LAYOUT_PRIORITY_H_
