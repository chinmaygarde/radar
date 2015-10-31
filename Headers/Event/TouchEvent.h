// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_EVENT_TOUCHEVENT_
#define RADARLOVE_EVENT_TOUCHEVENT_

#include <Event/Event.h>
#include <Geometry/Geometry.h>

#include <map>

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
  explicit TouchEvent(Identifier identifier,
                      const geom::Point& location,
                      Phase phase);

  /**
   *  Create a touch event from a channel message
   */
  explicit TouchEvent(core::Message& message);

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

  bool serialize(core::Message& m) const override;

  bool deserialize(core::Message& m) override;

 private:
  Identifier _identifier;
  geom::Point _location;
  Phase _phase;

  RL_DISALLOW_ASSIGN(TouchEvent);
};

}  // namespace event
}  // namespace rl

#endif  // RADARLOVE_EVENT_TOUCHEVENT_
