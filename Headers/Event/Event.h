// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_EVENT_EVENT__
#define __RADARLOVE_EVENT_EVENT__

#include "Core/Message.h"
#include "Core/Base.h"
#include "Core/Timing.h"

namespace rl {
class Event : public Serializable {
 public:
  /**
   *  Create an event with the given timestamp
   *
   *  @param timestamp the timestamp of the event
   */
  explicit Event(double timestamp = Time::Current());

  /**
   *  Get the timestamp of the event
   *
   *  @return the event timestamp
   */
  double timestamp() const;

  virtual void serialize(Message& m) const override;
  virtual void deserialize(Message& m) override;

 private:
  double _timestamp;
};
}

#endif /* defined(__RADARLOVE_EVENT_EVENT__) */
