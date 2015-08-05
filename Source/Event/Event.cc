// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Event/Event.h"

namespace rl {

Event::Event(double timestamp) : _timestamp(timestamp) {
}

double Event::timestamp() const {
  return _timestamp;
}

void Event::serialize(Message& m) const {
  m.encode(_timestamp);
}

void Event::deserialize(Message& m) {
  m.decode(_timestamp);
}

}  // namespace rl
