/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Event/Event.h>
#include <Geometry/Point.h>
#include <map>
#include <vector>

namespace rl {
namespace event {

class TouchEvent : public Event {
 public:
  enum Phase {
    Began,
    Moved,
    Ended,
    Cancelled,
  };

  using Identifier = uint64_t;
  using PhaseMap = std::map<Phase, std::vector<TouchEvent>>;

  /**
   *  Create a touch event with the given identifier and absolute location
   */
  TouchEvent(Identifier identifier, const geom::Point& location, Phase phase);

  /**
   *  Create a touch event from a channel message
   */
  TouchEvent(core::Message& message);

  /**
   *  Returns a touch identifier that is platform specific but is guaranteed to
   *  be unique per distinct touch in a multitouch sequence
   *
   *  @return the touch identifier
   */
  Identifier identifier() const;

  /**
   *  The absolute location of the touch. Coordinate space negotiation must be
   *  done separately and is not the responsibility of TouchEvent
   *
   *  @return the absolute touch location
   */
  const geom::Point& location() const;

  /**
   *  The phase of the touch
   *
   *  @return the touch phase
   */
  Phase phase() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

 private:
  Identifier _identifier;
  geom::Point _location;
  Phase _phase;

  RL_DISALLOW_ASSIGN(TouchEvent);
};

}  // namespace event
}  // namespace rl
