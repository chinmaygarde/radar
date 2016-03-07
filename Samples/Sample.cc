// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES

#include "Sample.h"

#include <Layout/ConstraintCreation.h>
#include <stdlib.h>

#define RND ((rand() % 100) / 100.0)

namespace sample {

SampleApplication::SampleApplication() {}

void SampleApplication::didFinishLaunching(
    rl::interface::Interface& interface) {}

static void AddPanRecognizer(rl::interface::Interface& interface,
                             rl::interface::ModelEntity& layer) {
  using Variable = rl::layout::Variable;

  Variable touchX(Variable::Proxy::Touch1, Variable::Property::PositionX);
  Variable touchY(Variable::Proxy::Touch1, Variable::Property::PositionY);

  auto layerX = layer | Variable::Property::PositionX;
  auto layerY = layer | Variable::Property::PositionY;

  auto constOffsetX = rl::layout::MakeConst(layerX - touchX);
  auto constOffsetY = rl::layout::MakeConst(layerY - touchY);

  interface.setupConstraints({
      layerX == touchX + constOffsetX,  //
      layerY == touchY + constOffsetY,  //
  });
}

void SampleApplication::didBecomeActive(rl::interface::Interface& interface) {
  using Property = rl::layout::Variable::Property;

  auto& root = interface.rootEntity();
  root.setBackgroundColor({0.2, 0.2, 0.2, 1.0});

  auto sub1 = interface.createEntity();
  sub1->setFrame({10.0, 10.0, 100.0, 100.0});
  sub1->setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root.addChild(*sub1);

  AddPanRecognizer(interface, *sub1);

  auto rootWidth = root | Property::BoundsWidth;
  auto rootHeight = root | Property::BoundsHeight;

  rl::interface::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::Type::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setDuration(rl::core::ClockDuration(0.5));
  action.setRepeatCount(rl::interface::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::interface::Entity::TransformationMask);

  interface.pushTransaction(std::move(action));

  const auto rows = 10;
  const auto cols = 10;

  for (double i = 0; i < rows; i++) {
    for (double j = 0; j < cols; j++) {
      auto layer = interface.createEntity();
      layer->setFrame({0.0, 0.0, 25, 25});
      layer->setBackgroundColor({RND, RND, RND, 1.0});

      auto rotation = rl::geom::Matrix::RotationZ(RND * M_PI * 2.0);
      layer->setTransformation(rotation);

      auto childPositionX = *layer | Property::PositionX;
      auto childPositionY = *layer | Property::PositionY;

      interface.setupConstraints({
          childPositionX == (i / rows) * rootWidth + 25,   //
          childPositionY == (j / cols) * rootHeight + 25,  //
      });

      root.addChild(*layer);
    }
  }

  interface.popTransaction();

  auto sub2 = interface.createEntity();
  sub2->setFrame({10.0, 10.0, 80.0, 80.0});
  sub2->setBackgroundColor({0.0, 1.0, 0.0, 1.0});
  sub1->addChild(*sub2);

  auto sub3 = interface.createEntity();
  sub3->setFrame({10.0, 10.0, 60.0, 60.0});
  sub3->setBackgroundColor({0.0, 0.0, 1.0, 1.0});
  sub2->addChild(*sub3);
}

void SampleApplication::didEnterBackground(
    rl::interface::Interface& interface) {}

void SampleApplication::didTerminate(rl::interface::Interface& interface) {}

void SampleApplication::didBecomeInactive(rl::interface::Interface& interface) {
}

}  // namespace sample
