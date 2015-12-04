// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Utilities.h>

namespace rl {
namespace layout {

bool NearZero(double value) {
  static const double epsilon = 1e-6;
  return value < 0.0 ? -value < epsilon : value < epsilon;
}

}  // namespace layout
}  // namespace rl
