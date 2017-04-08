/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Message.h>
#include <Event/Event.h>

namespace rl {
namespace event {

Event::Event(core::ClockPoint timestamp) : _timestamp(timestamp) {}

core::ClockPoint Event::timestamp() const {
  return _timestamp;
}

bool Event::serialize(core::Message& m) const {
  return m.encode(_timestamp);
}

bool Event::deserialize(core::Message& m, core::Namespace* ns) {
  return m.decode(_timestamp, ns);
}

}  // namespace event
}  // namespace rl
