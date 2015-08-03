// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_EVENT_TOUCHEVENT__
#define __RADARLOVE_EVENT_TOUCHEVENT__

#include "Event/Event.h"
#include "Geometry/Point.h"

namespace rl {
class TouchEvent : public Event {
 public:
  /**
   *  Create a touch event with the given identifier and absolute location
   */
  TouchEvent(uint64_t identifier, const Point& location);

  /**
   *  Returns a touch identifier that is platform specific but is guaranteed to
   *  be unique per distinct touch in a multitouch sequence
   *
   *  @return the touch identifier
   */
  uint64_t identifier() const;

  /**
   *  The absolute location of the touch. Coordinate space negotiation must be
   *  done separately and is not the responsibility of TouchEvent
   *
   *  @return the absolute touch location
   */
  const Point& location() const;

 private:
  const uint64_t _identifier;
  const Point _location;

  DISALLOW_COPY_AND_ASSIGN(TouchEvent);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_EVENT_TOUCHEVENT__) */
