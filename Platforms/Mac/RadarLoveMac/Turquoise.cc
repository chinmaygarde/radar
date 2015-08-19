// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Turquoise.h"

namespace tq {

TurquoiseApplication::TurquoiseApplication() {
}

void TurquoiseApplication::didFinishLaunching(rl::Interface& interface) {
  interface.touchEventChannel().setTouchEventCallback(
      [&](const std::vector<rl::TouchEvent>& touches,
          rl::TouchEvent::Phase phase) { onTouches(touches, phase); });
}

void TurquoiseApplication::didBecomeActive(rl::Interface& interface) {
  auto root = std::make_shared<rl::Layer>();
  root->setBackgroundColor({0.0, 0.0, 0.0, 1.0});
  interface.setRootLayer(root);

  auto sub1 = std::make_shared<rl::Layer>();
  sub1->setFrame({10.0, 10.0, 100.0, 100.0});
  sub1->setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root->addSublayer(sub1);

  auto sub2 = std::make_shared<rl::Layer>();
  sub2->setFrame({10.0, 10.0, 80.0, 80.0});
  sub2->setBackgroundColor({0.0, 1.0, 0.0, 1.0});
  sub1->addSublayer(sub2);

  auto sub3 = std::make_shared<rl::Layer>();
  sub3->setFrame({10.0, 10.0, 60.0, 60.0});
  sub3->setBackgroundColor({0.0, 0.0, 1.0, 1.0});
  sub2->addSublayer(sub3);
}

void TurquoiseApplication::didEnterBackground(rl::Interface& interface) {
}

void TurquoiseApplication::didTerminate(rl::Interface& interface) {
}

void TurquoiseApplication::didBecomeInactive(rl::Interface& interface) {
}

void TurquoiseApplication::didUpdateSize(rl::Interface& interface) {
}

void TurquoiseApplication::onTouches(const std::vector<rl::TouchEvent>& touches,
                                     rl::TouchEvent::Phase phase) {
}

}  // namespace tq
