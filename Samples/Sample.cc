// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Sample.h"

#include <stdlib.h>

namespace sample {

SampleApplication::SampleApplication() {
}

void SampleApplication::didFinishLaunching(rl::Interface& interface) {
}

void SampleApplication::didBecomeActive(rl::Interface& interface) {
  auto root = std::make_shared<rl::Layer>();
  root->setBackgroundColor({0.2, 0.2, 0.2, 1.0});
  interface.setRootLayer(root);

  rl::Action action;
  action.setTimingCurveType(rl::TimingCurve::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setDuration(1.0);
  action.setRepeatCount(rl::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::Entity::Transformation | rl::Entity::Opacity);
  interface.pushTransaction(std::move(action));

  for (auto i = 0; i < 1000; i++) {
    auto layer = std::make_shared<rl::Layer>();
    layer->setFrame({static_cast<double>(rand() % 1600),
                     static_cast<double>(rand() % 1200),
                     static_cast<double>(10 + rand() % 120),
                     static_cast<double>(10 + rand() % 120)});
    layer->setTransformation(
        rl::Matrix::RotationZ(((rand() % 10) / 10.0) * M_PI * 2.0));
    layer->setBackgroundColor({(rand() % 100) / 100.0,
                               (rand() % 100) / 100.0,
                               (rand() % 100) / 100.0,
                               1.0});
    layer->setOpacity(((rand() % 10) / 10.0));
    root->addSublayer(layer);
  }

  interface.popTransaction();

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

void SampleApplication::didEnterBackground(rl::Interface& interface) {
}

void SampleApplication::didTerminate(rl::Interface& interface) {
}

void SampleApplication::didBecomeInactive(rl::Interface& interface) {
}

void SampleApplication::didUpdateSize(rl::Interface& interface) {
}

}  // namespace sample
