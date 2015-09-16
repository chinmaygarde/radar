// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/GestureRecognizer.h>

namespace rl {

GestureRecognizer::GestureRecognizer(Equation&& equation)
    : _equation(std::move(equation)) {
}

bool GestureRecognizer::serialize(Message& message) const {
  return false;
}

bool GestureRecognizer::deserialize(Message& message) {
  return false;
}

}  // namespace rl
