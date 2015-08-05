// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Event/TouchEvent.h"

namespace rl {

TouchEvent::TouchEvent(uint64_t identifier, const Point& location, Phase phase)
    : Event(), _identifier(identifier), _location(location), _phase(phase) {
}

uint64_t TouchEvent::identifier() const {
  return _identifier;
}

const Point& TouchEvent::location() const {
  return _location;
}

Message TouchEvent::serialize() const {
  Message m(sizeof(TouchEvent));
  m.encode(_identifier);
  m.encode(_location);
  m.encode(_phase);
  return m;
}

void TouchEvent::deserialize(Message& m) {
  m.decode(_identifier);
  m.decode(_location);
  m.decode(_phase);
}

}  // namespace rl
