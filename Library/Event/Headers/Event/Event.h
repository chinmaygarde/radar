// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_EVENT_EVENT_
#define RADAR_EVENT_EVENT_

#include <Core/MessageSerializable.h>

namespace rl {
namespace event {

class Event : public core::MessageSerializable {
 public:
  /**
   *  Create an event with the given timestamp
   *
   *  @param timestamp the timestamp of the event
   */
  Event(core::ClockPoint timestamp);

  /**
   *  Get the timestamp of the event
   *
   *  @return the event timestamp
   */
  core::ClockPoint timestamp() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

 private:
  core::ClockPoint _timestamp;
};

}  // namespace event
}  // namespace rl

#endif  // RADAR_EVENT_EVENT_
