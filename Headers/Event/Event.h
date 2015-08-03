// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_EVENT_EVENT__
#define __RADARLOVE_EVENT_EVENT__

#include "Core/Base.h"
#include "Core/Time.h"

namespace rl {
class Event {
 public:
  /**
   *  Create an event with the given timestamp
   *
   *  @param timestamp the timestamp of the event
   */
  Event(double timestamp = Time::Current());

  /**
   *  Get the timestamp of the event
   *
   *  @return the event timestamp
   */
  double timestamp() const;

 private:
  const double _timestamp;

  DISALLOW_COPY_AND_ASSIGN(Event);
};
}

#endif /* defined(__RADARLOVE_EVENT_EVENT__) */
