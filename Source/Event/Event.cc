// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/Event.h>

namespace rl {

Event::Event(ClockPoint timestamp) : _timestamp(timestamp) {
}

ClockPoint Event::timestamp() const {
  return _timestamp;
}

bool Event::serialize(Message& m) const {
  return m.encode(_timestamp);
}

bool Event::deserialize(Message& m) {
  return m.decode(_timestamp);
}

}  // namespace rl
