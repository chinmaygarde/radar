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
  root->setBackgroundColor({0.0, 0.0, 0.0, 0.0});
  interface.setRootLayer(root);

  auto sub1 = std::make_shared<rl::Layer>();
  sub1->setFrame({10.0, 20.0, 30.0, 40.0});

  auto sub2 = std::make_shared<rl::Layer>();
  sub2->setFrame({50.0, 60.0, 70.0, 80.0});

  root->addSublayer(sub1);
  root->addSublayer(sub2);
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
