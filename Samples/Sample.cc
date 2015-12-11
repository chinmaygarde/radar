// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES

#include "Sample.h"

#include <Layout/ConstraintCreation.h>
#include <stdlib.h>

namespace sample {

SampleApplication::SampleApplication() {}

void SampleApplication::didFinishLaunching(
    rl::interface::Interface& interface) {}

static void AddPanRecognizer(rl::interface::Interface& interface,
                             rl::interface::Layer& layer) {
  using Variable = rl::layout::Variable;

  Variable touchX(Variable::Proxy::Touch0, Variable::Property::PositionX);
  Variable touchY(Variable::Proxy::Touch0, Variable::Property::PositionY);

  interface.setupConstraints({
      (layer | Variable::Property::PositionX) == touchX,  //
      (layer | Variable::Property::PositionY) == touchY,  //
  });
}

static void AddDockedPanel(rl::interface::Interface& interface) {
  using Property = rl::layout::Variable::Property;

  auto layer = interface.rootLayer();
  auto child = std::make_shared<rl::interface::Layer>();

  child->setBackgroundColor(rl::coordinator::ColorBlue);
  child->setFrame({35, 35, 200, 200});

  layer->addSublayer(child);

  auto containerPositionX = *layer | Property::PositionX;
  auto childPositionX = *child | Property::PositionX;
  auto containerPositionY = *layer | Property::PositionY;
  auto childPositionY = *child | Property::PositionY;
  auto childWidth = *child | Property::BoundsWidth;
  auto containerWidth = *layer | Property::BoundsWidth;

  interface.setupConstraints({
      containerPositionX == childPositionX,  //
      containerPositionY == childPositionY,  //
      childWidth == 0.75 * containerWidth,   //
  });
}

void SampleApplication::didBecomeActive(rl::interface::Interface& interface) {
  using Property = rl::layout::Variable::Property;

  auto root = std::make_shared<rl::interface::Layer>();
  root->setBackgroundColor({0.2, 0.2, 0.2, 1.0});
  interface.setRootLayer(root);

  auto sub1 = std::make_shared<rl::interface::Layer>();
  sub1->setFrame({10.0, 10.0, 100.0, 100.0});
  sub1->setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root->addSublayer(sub1);
  AddPanRecognizer(interface, *sub1);

  auto draggedX = *sub1 | Property::PositionX;
  auto draggedY = *sub1 | Property::PositionY;

  auto rootWidth = *root | Property::BoundsWidth;
  auto rootHeight = *root | Property::BoundsHeight;

  rl::interface::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setDuration(rl::core::ClockDuration(0.5));
  action.setRepeatCount(rl::interface::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::interface::Entity::Transformation);

  interface.pushTransaction(std::move(action));

  const auto rows = 5;
  const auto cols = 5;
  const auto delta = 100;

  for (auto i = 0; i < rows; i++) {
    for (auto j = 0; j < cols; j++) {
      auto layer = std::make_shared<rl::interface::Layer>();
      layer->setFrame({0.0, 0.0, 25, 25});
      layer->setBackgroundColor({(rand() % 100) / 100.0, (rand() % 100) / 100.0,
                                 (rand() % 100) / 100.0, 1.0});
      layer->setTransformation(
          rl::geom::Matrix::RotationZ(((rand() % 10) / 10.0) * M_PI * 2.0));

      auto childPositionX = *layer | Property::PositionX;
      auto childPositionY = *layer | Property::PositionY;

      interface.setupConstraints({
          childPositionX == ((i + 1) / (double)rows) * rootWidth |
              rl::layout::priority::Medium,  //
          childPositionY == ((j + 1) / (double)cols) * rootHeight |
              rl::layout::priority::Medium,  //
      });

      root->addSublayer(layer);
    }
  }

  interface.popTransaction();

  auto sub2 = std::make_shared<rl::interface::Layer>();
  sub2->setFrame({10.0, 10.0, 80.0, 80.0});
  sub2->setBackgroundColor({0.0, 1.0, 0.0, 1.0});
  sub1->addSublayer(sub2);

  auto sub3 = std::make_shared<rl::interface::Layer>();
  sub3->setFrame({10.0, 10.0, 60.0, 60.0});
  sub3->setBackgroundColor({0.0, 0.0, 1.0, 1.0});
  sub2->addSublayer(sub3);

  //  AddDockedPanel(interface);
}

void SampleApplication::didEnterBackground(
    rl::interface::Interface& interface) {}

void SampleApplication::didTerminate(rl::interface::Interface& interface) {}

void SampleApplication::didBecomeInactive(rl::interface::Interface& interface) {
}

void SampleApplication::didUpdateSize(rl::interface::Interface& interface) {}

}  // namespace sample
