// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES

#include "Sample.h"

#include <Layout/ConstraintCreation.h>
#include <Recognition/GestureRecognizer.h>
#include <stdlib.h>

namespace sample {

SampleApplication::SampleApplication() {}

void SampleApplication::didFinishLaunching(
    rl::interface::Interface& interface) {}

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

  rl::layout::Variable containerPositionX(
      layer->identifier(), rl::layout::Variable::Property::PositionX);
  rl::layout::Variable childPositionX(
      child->identifier(), rl::layout::Variable::Property::PositionX);
  rl::layout::Variable containerPositionY(
      layer->identifier(), rl::layout::Variable::Property::PositionY);
  rl::layout::Variable childPositionY(
      child->identifier(), rl::layout::Variable::Property::PositionY);
  rl::layout::Variable childWidth(child->identifier(),
                                  rl::layout::Variable::Property::BoundsWidth);
  rl::layout::Variable containerWidth(
      layer->identifier(), rl::layout::Variable::Property::BoundsWidth);

  interface.setupConstraints({
      containerPositionX == childPositionX,  //
      containerPositionY == childPositionY,  //
      childWidth == 0.75 * containerWidth,   //
  });
}

void SampleApplication::didBecomeActive(rl::interface::Interface& interface) {
  auto root = std::make_shared<rl::interface::Layer>();
  root->setBackgroundColor({0.2, 0.2, 0.2, 1.0});
  interface.setRootLayer(root);

  rl::layout::Variable rootWidth(root->identifier(),
                                 rl::layout::Variable::Property::BoundsWidth);
  rl::layout::Variable rootHeight(root->identifier(),
                                  rl::layout::Variable::Property::BoundsHeight);

  rl::interface::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setDuration(rl::core::ClockDuration(0.5));
  action.setRepeatCount(rl::interface::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::interface::Entity::Transformation);

  interface.pushTransaction(std::move(action));

  const auto rows = 25;
  const auto cols = 25;

  for (auto i = 0; i < rows; i++) {
    for (auto j = 0; j < cols; j++) {
      auto layer = std::make_shared<rl::interface::Layer>();
      layer->setFrame({0.0, 0.0, 25, 25});
      layer->setBackgroundColor({(rand() % 100) / 100.0, (rand() % 100) / 100.0,
                                 (rand() % 100) / 100.0, 1.0});
      layer->setTransformation(
          rl::geom::Matrix::RotationZ(((rand() % 10) / 10.0) * M_PI * 2.0));

      rl::layout::Variable childPositionX(
          layer->identifier(), rl::layout::Variable::Property::PositionX);
      rl::layout::Variable childPositionY(
          layer->identifier(), rl::layout::Variable::Property::PositionY);

      interface.setupConstraints({
          childPositionX == ((i + 1) / (double)rows) * rootWidth,   //
          childPositionY == ((j + 1) / (double)cols) * rootHeight,  //
      });

      root->addSublayer(layer);
    }
  }

  interface.popTransaction();

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

  AddDockedPanel(interface);
}

void SampleApplication::didEnterBackground(
    rl::interface::Interface& interface) {}

void SampleApplication::didTerminate(rl::interface::Interface& interface) {}

void SampleApplication::didBecomeInactive(rl::interface::Interface& interface) {
}

void SampleApplication::didUpdateSize(rl::interface::Interface& interface) {}

}  // namespace sample
