// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Event/TouchEvent.h"

namespace rl {

TouchEvent::TouchEvent(uint64_t identifier, const Point& location)
    : Event(), _identifier(identifier), _location(location) {
}

uint64_t TouchEvent::identifier() const {
  return _identifier;
}

const Point& TouchEvent::location() const {
  return _location;
}

}  // namespace rl
