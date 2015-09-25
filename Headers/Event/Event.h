// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_EVENT_EVENT__
#define __RADARLOVE_EVENT_EVENT__

#include <Core/Core.h>

namespace rl {
class Event : public Serializable {
 public:
  /**
   *  Create an event with the given timestamp
   *
   *  @param timestamp the timestamp of the event
   */
  explicit Event(ClockPoint timestamp);

  /**
   *  Get the timestamp of the event
   *
   *  @return the event timestamp
   */
  ClockPoint timestamp() const;

  virtual bool serialize(Message& m) const override;
  virtual bool deserialize(Message& m) override;

 private:
  ClockPoint _timestamp;
};
}

#endif  // __RADARLOVE_EVENT_EVENT__
