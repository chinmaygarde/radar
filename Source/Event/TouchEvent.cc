// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/TouchEvent.h>

namespace rl {

TouchEvent::TouchEvent(uint64_t identifier, const Point& location, Phase phase)
    : Event(Time::ToSeconds(Time::Current())),
      _identifier(identifier),
      _location(location),
      _phase(phase) {
}

TouchEvent::TouchEvent(Message& message) : Event(0.0), _location(0.0, 0.0) {
  deserialize(message);
  RL_ASSERT(message.size() == message.sizeRead());
}

uint64_t TouchEvent::identifier() const {
  return _identifier;
}

const Point& TouchEvent::location() const {
  return _location;
}

TouchEvent::Phase TouchEvent::phase() const {
  return _phase;
}

void TouchEvent::serialize(Message& m) const {
  Event::serialize(m);
  m.encode(_identifier);
  m.encode(_location);
  m.encode(_phase);
}

void TouchEvent::deserialize(Message& m) {
  Event::deserialize(m);
  m.decode(_identifier);
  m.decode(_location);
  m.decode(_phase);
}

}  // namespace rl
