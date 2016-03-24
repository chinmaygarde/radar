// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/TouchEvent.h>

namespace rl {
namespace event {

TouchEvent::TouchEvent(Identifier identifier,
                       const geom::Point& location,
                       Phase phase)
    : Event(core::Clock::now()),
      _identifier(identifier),
      _location(location),
      _phase(phase) {}

TouchEvent::TouchEvent(core::Message& message)
    : Event(core::ClockPoint::min()), _location(0.0, 0.0) {
  deserialize(message, nullptr);
  RL_ASSERT(message.size() == message.sizeRead());
}

TouchEvent::Identifier TouchEvent::identifier() const {
  return _identifier;
}

const geom::Point& TouchEvent::location() const {
  return _location;
}

TouchEvent::Phase TouchEvent::phase() const {
  return _phase;
}

bool TouchEvent::serialize(core::Message& m) const {
  bool result = true;
  result &= Event::serialize(m);
  result &= m.encode(_identifier);
  result &= m.encode(_location);
  result &= m.encode(_phase);
  return result;
}

bool TouchEvent::deserialize(core::Message& message, core::Namespace* ns) {
  bool result = true;
  result &= Event::deserialize(message, ns);
  result &= message.decode(_identifier, ns);
  result &= message.decode(_location, ns);
  result &= message.decode(_phase, ns);
  return result;
}

}  // namespace event
}  // namespace rl
