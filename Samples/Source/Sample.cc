/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#define _USE_MATH_DEFINES

#include <Samples/Sample.h>

#include <Geometry/PathBuilder.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>
#include <Layout/ConstraintCreation.h>
#include <stdlib.h>

#define RND ((rand() % 100) / 100.0)

namespace sample {

SampleApplication::SampleApplication() {}

void SampleApplication::didFinishLaunching(
    rl::interface::Interface& interface) {}

static void AddPanRecognizer(rl::interface::Interface& interface,
                             rl::interface::ModelEntity& layer) {
  using Variable = rl::expr::Variable;

  Variable touchX(Variable::Proxy::Touch1, Variable::Property::PositionX);
  Variable touchY(Variable::Proxy::Touch1, Variable::Property::PositionY);

  Variable layerX(layer.identifier(), Variable::Property::PositionX);
  Variable layerY(layer.identifier(), Variable::Property::PositionY);

  auto constOffsetX = MakeConst(layerX - touchX);
  auto constOffsetY = MakeConst(layerY - touchY);

  interface.setupConstraints({
      layerX == touchX + constOffsetX,  //
      layerY == touchY + constOffsetY,  //
  });
}

std::set<rl::interface::ModelEntity::Ref> AddLayers(
    rl::interface::Interface& interface) {
  using Property = rl::expr::Variable::Property;
  auto& root = interface.rootEntity();

  auto rootWidth = root | Property::BoundsWidth;
  auto rootHeight = root | Property::BoundsHeight;

  const auto rows = 15;
  const auto cols = 15;

  std::set<rl::interface::ModelEntity::Ref> layers;

  for (double i = 0; i < rows; i++) {
    for (double j = 0; j < cols; j++) {
      auto layer = interface.createEntity();
      layer->setFrame(
          {0.0, 0.0, fmax(RND * 100.0, 25.0), fmax(RND * 100.0, 25.0)});
      layer->setBackgroundColor({RND, RND, RND, 1.0});
      layer->setStrokeColor({RND, RND, RND, 1.0});
      layer->setStrokeSize(RND * 10.0);
      layer->setTransformation(
          rl::geom::Matrix::RotationZ(RND * M_PI * 2.0 * 3.0));

      auto childPositionX = *layer | Property::PositionX;
      auto childPositionY = *layer | Property::PositionY;

      interface.setupConstraints({
          childPositionX == (i / rows) * rootWidth + 25,   //
          childPositionY == (j / cols) * rootHeight + 25,  //
      });

      root.addChild(layer);

      layers.emplace(std::move(layer));
    }
  }
  return layers;
}

void AddGridToRoot(rl::interface::Interface& interface) {
  auto layers = AddLayers(interface);

  rl::animation::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::Type::EaseInEaseOut);
  action.setAutoReverses(true);
  action.setRepeatCount(rl::animation::Action::RepeatCountInfinity);

  // Update transformation.
  action.setDuration(rl::core::ClockDuration(1.5));
  action.setPropertyMask(rl::entity::Entity::TransformationMask);
  auto popTransformation = interface.pushTransaction(std::move(action));
  for (const auto& layer : layers) {
    layer->setTransformation(
        rl::geom::Matrix::RotationZ(RND * M_PI * 2.0 * 3.0));
  }

  // Update stroke sizes.
  action.setDuration(rl::core::ClockDuration(0.25));
  action.setPropertyMask(rl::entity::Entity::StrokeSizeMask);
  auto popStrokeSizes = interface.pushTransaction(std::move(action));
  for (const auto& layer : layers) {
    layer->setStrokeSize(RND * 10.0);
  }

  // Update colors.
  action.setDuration(rl::core::ClockDuration(3.0));
  action.setPropertyMask(rl::entity::Entity::BackgroundColorMask);
  auto popColor = interface.pushTransaction(std::move(action));
  for (const auto& layer : layers) {
    layer->setBackgroundColor({RND, RND, RND, 1.0});
  }
}

void AddDraggableEntity(rl::interface::Interface& interface) {
  auto& root = interface.rootEntity();

  auto sub1 = interface.createEntity();
  sub1->setFrame({10.0, 325.0, 100.0, 100.0});
  sub1->setBackgroundColor({1.0, 0.0, 0.0, 1.0});
  root.addChild(sub1);

  auto sub2 = interface.createEntity();
  sub2->setFrame({10.0, 10.0, 80.0, 80.0});
  sub2->setBackgroundColor({0.0, 1.0, 0.0, 1.0});
  sub1->addChild(sub2);

  auto sub3 = interface.createEntity();
  sub3->setFrame({10.0, 10.0, 60.0, 60.0});
  sub3->setBackgroundColor({0.0, 0.0, 1.0, 1.0});
  sub2->addChild(sub3);

  AddPanRecognizer(interface, *sub1);
}

void AddRadialArrangement(rl::interface::Interface& interface) {
  using Property = rl::expr::Variable::Property;

  auto& root = interface.rootEntity();

  auto rootW = root | Property::BoundsWidth;
  auto rootH = root | Property::BoundsHeight;

  for (auto i = 0, count = 25; i < count; i++) {
    auto entity = interface.createEntity();

    entity->setFrame({0.0, 0.0, 15.0, 15.0});
    entity->setBackgroundColor({1.0, 1.0, 1.0, 1.0});

    auto posX = *entity | Property::PositionX;
    auto posY = *entity | Property::PositionY;

    auto angle = 2.0 * ((double)i / count) * M_PI;

    interface.setupConstraints({
        posX == (rootW / 2.0) + (rootW * 0.25 * cos(angle)),  //
        posY == (rootH / 2.0) + (rootH * 0.25 * sin(angle)),  //
    });

    root.addChild(entity);
  }
}

void AddEntityWithPath(rl::interface::Interface& interface) {
  auto entity = interface.createEntity();

  entity->setBackgroundColor(rl::entity::Color::White());

  rl::geom::PathBuilder builder;

  builder.addRoundedRect({{325, 175}, {200, 200}}, {80, 20, 50, 20});

  entity->setPath(builder.path());

  interface.rootEntity().addChild(entity);
}

void AddSVG(rl::interface::Interface& interface,
            const rl::core::Bundle& bundle) {
  rl::core::FileHandle archiveHandle(
      bundle.uriForResource(rl::core::URI{"Simple.svg"}));

  auto archive = rl::ib::InterfaceBuilderArchive::Make(archiveHandle);

  if (archive == nullptr || !archive->isValid()) {
    return;
  }

  auto entity = interface.createEntity();
  rl::ib::InterfaceBuilderArchive::EntityMap map;
  interface.rootEntity().addChild(archive->inflate(interface, map));
}

void AddStrokedPath(rl::interface::Interface& interface) {
  rl::geom::PathBuilder builder;

  builder.moveTo({200, 200});
  builder.lineTo({400, 400});

  builder.moveTo({200, 400});
  builder.lineTo({400, 200});

  builder.addRoundedRect({100, 100, 400, 400}, 50);

  builder.moveTo({60, 10});
  builder.cubicCurveTo({1200, 700}, {100, 1200}, {1600, 100});

  auto entity = interface.createEntity();
  entity->setFrame({100.00, 100.00, 100, 100});
  entity->setPath(builder.path());
  entity->setStrokeColor({1.0, 1.0, 1.0, 1.0});
  entity->setStrokeSize(4);
  interface.rootEntity().addChild(entity);
}

void SampleApplication::AddImageWithRoundedRect(
    rl::interface::Interface& interface) {
  auto entity = interface.createEntity();

  /*
   *  Set the rounded rect.
   */
  rl::geom::PathBuilder builder;
  builder.addRoundedRect({{10, 10}, {300, 400}}, {50, 50, 50, 50});
  entity->setPath(builder.path());
  entity->setFrame({10, 500, 0.0, 0.0});

  /*
   *  Set the image.
   */
  auto imageURI = _bundle.uriForResource(rl::core::URI{"MonaLisa.jpg"});
  entity->setContents(rl::image::Image{std::move(imageURI)});
  entity->setStrokeSize(10.0);
  entity->setStrokeColor(rl::entity::Color::Teal());

  interface.rootEntity().addChild(entity);
}

void AddStrokedBox(rl::interface::Interface& interface) {
  auto entity = interface.createEntity();

  entity->setBackgroundColor(rl::entity::Color::Turquoise());
  entity->setFrame({200, 200, 400, 400});
  entity->setStrokeColor(rl::entity::Color::White());
  entity->setStrokeSize(2);

  interface.rootEntity().addChild(entity);
}

void SampleApplication::AddEntityWithImage(
    rl::interface::Interface& interface) {
  rl::animation::Action action;
  action.setTimingCurveType(rl::animation::TimingCurve::Type::EaseInEaseOut);
  action.setDuration(rl::core::ClockDuration(0.5));
  action.setAutoReverses(true);
  action.setRepeatCount(rl::animation::Action::RepeatCountInfinity);
  action.setPropertyMask(rl::entity::Entity::TransformationMask);

  auto pop = interface.pushTransaction(std::move(action));

  auto entity = interface.createEntity();

  entity->setFrame({100, 100, 100, 100});

  auto imageURI = _bundle.uriForResource(rl::core::URI{"Beachball.png"});

  entity->setContents(rl::image::Image{std::move(imageURI)});
  entity->setTransformation(rl::geom::Matrix::RotationZ(M_PI));

  interface.rootEntity().addChild(entity);
}

void SampleApplication::didBecomeActive(rl::interface::Interface& interface) {
  auto& root = interface.rootEntity();
  root.setBackgroundColor({0.2, 0.2, 0.2, 1.0});

  AddGridToRoot(interface);

  AddDraggableEntity(interface);

  AddRadialArrangement(interface);

  AddEntityWithImage(interface);

  AddEntityWithPath(interface);

  AddImageWithRoundedRect(interface);

  AddSVG(interface, _bundle);

  AddStrokedPath(interface);
}

void SampleApplication::didEnterBackground(
    rl::interface::Interface& interface) {}

void SampleApplication::didTerminate(rl::interface::Interface& interface) {}

void SampleApplication::didBecomeInactive(rl::interface::Interface& interface) {
}

}  // namespace sample
