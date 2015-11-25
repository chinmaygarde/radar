// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES

#include "Sample.h"

#include <stdlib.h>
#include <Recognition/GestureRecognizer.h>
#include <Layout/ConstraintCreation.h>

namespace sample {

SampleApplication::SampleApplication() {
}

void SampleApplication::didFinishLaunching(
    rl::interface::Interface& interface) {
}

static void AddPanRecognizer(rl::interface::Interface& interface,
                             rl::interface::Layer& layer) {
  rl::recognition::Variable y(layer, rl::interface::Entity::Position);
  rl::recognition::Variable x(rl::recognition::Variable::Proxy::Touch1,
                              rl::interface::Entity::Position);

  rl::recognition::Term::VariableDegree variableDegree(x, 1);
  rl::recognition::Term term(1.0, {variableDegree});
  rl::recognition::Polynomial polynomial({term}, 0.0);

  rl::recognition::GestureRecognizer recognizer(std::move(y),
                                                std::move(polynomial));

  bool result = interface.setupGestureRecognizer(std::move(recognizer));
  RL_ASSERT(result);
}

static void AddDockedPanel(rl::interface::Interface& interface) {
  auto layer = interface.rootLayer();
  auto child = std::make_shared<rl::interface::Layer>();

  child->setBackgroundColor(rl::coordinator::ColorBlue);
  child->setFrame({35, 35, 200, 200});

  layer->addSublayer(child);

  rl::layout::Variable containerPosition(
      layer->identifier(), rl::interface::Entity::Property::Position);
  rl::layout::Variable childPosition(child->identifier(),
                                     rl::interface::Entity::Property::Position);

  interface.setupConstraints({
      containerPosition == childPosition,  //
  });
}

void SampleApplication::didBecomeActive(rl::interface::Interface& interface) {
  auto root = std::make_shared<rl::interface::Layer>();
  root->setBackgroundColor({0.2, 0.2, 0.2, 1.0});
  interface.setRootLayer(root);

  rl::interface::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setDuration(rl::core::ClockDuration(1.0));
  action.setRepeatCount(rl::interface::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::interface::Entity::Transformation |
                         rl::interface::Entity::Opacity);
  interface.pushTransaction(std::move(action));

  for (auto i = 0; i < 500; i++) {
    auto layer = std::make_shared<rl::interface::Layer>();
    layer->setFrame({static_cast<double>(rand() % 1600),
                     static_cast<double>(rand() % 1200),
                     static_cast<double>(10 + rand() % 120),
                     static_cast<double>(10 + rand() % 120)});
    layer->setTransformation(
        rl::geom::Matrix::RotationZ(((rand() % 10) / 10.0) * M_PI * 2.0));
    layer->setBackgroundColor({(rand() % 100) / 100.0,
                               (rand() % 100) / 100.0,
                               (rand() % 100) / 100.0,
                               1.0});
    layer->setOpacity(((rand() % 10) / 10.0));
    root->addSublayer(layer);
  }

  interface.popTransaction();

  AddDockedPanel(interface);

  auto sub1 = std::make_shared<rl::interface::Layer>();
  sub1->setFrame({10.0, 10.0, 100.0, 100.0});
  sub1->setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root->addSublayer(sub1);
  AddPanRecognizer(interface, *sub1);

  auto sub2 = std::make_shared<rl::interface::Layer>();
  sub2->setFrame({10.0, 10.0, 80.0, 80.0});
  sub2->setBackgroundColor({0.0, 1.0, 0.0, 1.0});
  sub1->addSublayer(sub2);
  AddPanRecognizer(interface, *sub2);

  auto sub3 = std::make_shared<rl::interface::Layer>();
  sub3->setFrame({10.0, 10.0, 60.0, 60.0});
  sub3->setBackgroundColor({0.0, 0.0, 1.0, 1.0});
  sub2->addSublayer(sub3);
  AddPanRecognizer(interface, *sub3);
}

void SampleApplication::didEnterBackground(
    rl::interface::Interface& interface) {
}

void SampleApplication::didTerminate(rl::interface::Interface& interface) {
}

void SampleApplication::didBecomeInactive(rl::interface::Interface& interface) {
}

void SampleApplication::didUpdateSize(rl::interface::Interface& interface) {
}

}  // namespace sample
