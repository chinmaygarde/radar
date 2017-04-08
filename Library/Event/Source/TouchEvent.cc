/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Message.h>
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
  RL_RETURN_IF_FALSE(Event::serialize(m));
  RL_RETURN_IF_FALSE(m.encode(_identifier));
  RL_RETURN_IF_FALSE(m.encode(_location));
  RL_RETURN_IF_FALSE(m.encode(_phase));
  return true;
}

bool TouchEvent::deserialize(core::Message& message, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(Event::deserialize(message, ns));
  RL_RETURN_IF_FALSE(message.decode(_identifier, ns));
  RL_RETURN_IF_FALSE(message.decode(_location, ns));
  RL_RETURN_IF_FALSE(message.decode(_phase, ns));
  return true;
}

}  // namespace event
}  // namespace rl
