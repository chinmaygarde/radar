// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Turquoise.h"

namespace tq {

TurquoiseApplication::TurquoiseApplication() {
}

void TurquoiseApplication::didFinishLaunching(rl::Interface& interface) {
}

void TurquoiseApplication::didBecomeActive(rl::Interface& interface) {
  auto root = std::make_shared<rl::Layer>();
  root->setBackgroundColor({0.0, 0.0, 0.0, 1.0});
  interface.setRootLayer(root);

  auto sub1 = std::make_shared<rl::Layer>();
  sub1->setFrame({10.0, 10.0, 100.0, 100.0});
  sub1->setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root->addSublayer(sub1);
}

void TurquoiseApplication::didEnterBackground(rl::Interface& interface) {
}

void TurquoiseApplication::didTerminate(rl::Interface& interface) {
}

void TurquoiseApplication::didBecomeInactive(rl::Interface& interface) {
}

void TurquoiseApplication::didUpdateSize(rl::Interface& interface) {
}

}  // namespace tq
