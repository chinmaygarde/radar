// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Interface/Interface.h"

using namespace rl;

void Interface::run() {
  if (_looper != nullptr) {
    return;
  }

  _looper = Looper::Current();
  _looper->loop();
}
