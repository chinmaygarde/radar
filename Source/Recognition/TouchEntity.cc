// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/TouchEntity.h>
#include <Event/TouchEvent.h>

namespace rl {

TouchEntity::TouchEntity(const TouchEvent& event) : Entity(false) {
  setBounds({0.0, 0.0, 1.0, 1.0});
  setPosition(event.location());
}

TouchEntity::~TouchEntity() {
}

}  // namespace rl
