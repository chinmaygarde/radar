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

static void AddGridToRoot(rl::interface::Interface& interface) {
  using Property = rl::layout::Variable::Property;
  auto& root = interface.rootEntity();

  auto rootWidth = root | Property::BoundsWidth;
  auto rootHeight = root | Property::BoundsHeight;

  rl::animation::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::Type::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setDuration(rl::core::ClockDuration(0.5));
  action.setRepeatCount(rl::animation::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::entity::Entity::TransformationMask);

  interface.pushTransaction(std::move(action));

  const auto rows = 25;
  const auto cols = 25;

  for (double i = 0; i < rows; i++) {
    for (double j = 0; j < cols; j++) {
      auto layer = interface.createEntity();
      layer.setFrame({0.0, 0.0, 25, 25});
      layer.setBackgroundColor({RND, RND, RND, 1.0});

      auto rotation = rl::geom::Matrix::RotationZ(RND * M_PI * 2.0);
      layer.setTransformation(rotation);

      auto childPositionX = layer | Property::PositionX;
      auto childPositionY = layer | Property::PositionY;

      interface.setupConstraints({
          childPositionX == (i / rows) * rootWidth + 25,   //
          childPositionY == (j / cols) * rootHeight + 25,  //
      });

      root.addChild(layer);
    }
  }

  interface.popTransaction();
}

static void AddDraggableEntity(rl::interface::Interface& interface) {
  using Property = rl::layout::Variable::Property;

  auto& root = interface.rootEntity();
  root.setBackgroundColor({0.2, 0.2, 0.2, 1.0});

  auto sub1 = interface.createEntity();
  sub1.setFrame({10.0, 10.0, 100.0, 100.0});
  sub1.setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root.addChild(sub1);

  auto sub2 = interface.createEntity();
  sub2.setFrame({10.0, 10.0, 80.0, 80.0});
  sub2.setBackgroundColor({0.0, 1.0, 0.0, 1.0});
  sub1.addChild(sub2);

  auto sub3 = interface.createEntity();
  sub3.setFrame({10.0, 10.0, 60.0, 60.0});
  sub3.setBackgroundColor({0.0, 0.0, 1.0, 1.0});
  sub2.addChild(sub3);

  AddPanRecognizer(interface, sub1);
}

static void AddRadialArrangement(rl::interface::Interface& interface) {
  using Property = rl::layout::Variable::Property;

  auto& root = interface.rootEntity();

  auto rootW = root | Property::BoundsWidth;
  auto rootH = root | Property::BoundsHeight;

  for (auto i = 0, count = 25; i < count; i++) {
    auto entity = interface.createEntity();

    entity.setFrame({0.0, 0.0, 15.0, 15.0});
    entity.setBackgroundColor({1.0, 1.0, 1.0, 1.0});

    auto posX = entity | Property::PositionX;
    auto posY = entity | Property::PositionY;

    auto angle = 2.0 * ((double)i / count) * M_PI;

    interface.setupConstraints({
        posX == (rootW / 2.0) + (rootW * 0.25 * cos(angle)),  //
        posY == (rootH / 2.0) + (rootH * 0.25 * sin(angle)),  //
    });

    root.addChild(entity);
  }
}

void SampleApplication::AddEntityWithImage(
    rl::interface::Interface& interface) {
  auto entity = interface.createEntity();

  entity.setFrame({100, 100, 100, 100});

  auto imageURI = _bundle.uriForResource(rl::core::URI{"Beachball.png"});

  entity.setContents(rl::image::Image{std::move(imageURI)});

  interface.rootEntity().addChild(entity);
}

void SampleApplication::didBecomeActive(rl::interface::Interface& interface) {
  AddGridToRoot(interface);

  AddDraggableEntity(interface);

  AddRadialArrangement(interface);

  AddEntityWithImage(interface);
}

void SampleApplication::didEnterBackground(
    rl::interface::Interface& interface) {}

void SampleApplication::didTerminate(rl::interface::Interface& interface) {}

void SampleApplication::didBecomeInactive(rl::interface::Interface& interface) {
}

}  // namespace sample
