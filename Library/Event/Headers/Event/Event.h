/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
