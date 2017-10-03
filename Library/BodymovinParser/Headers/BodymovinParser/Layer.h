/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <BodymovinParser/Effect.h>
#include <BodymovinParser/Mask.h>
#include <BodymovinParser/Shape.h>
#include <BodymovinParser/TextData.h>
#include <BodymovinParser/Transform.h>
#include <BodymovinParser/Types.h>
#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <string>
#include <vector>

namespace rl {
namespace bodymovin {

class Layer {
 public:
  Layer();

  ~Layer();

  const Transform& transform() const;

  bool shouldAutoOrient() const;

  BlendMode blendMode() const;

  bool is3DLayer() const;

  int64_t index() const;

  const std::string& className() const;

  const std::string& layerHtmlID() const;

  double inPoint() const;

  double outPoint() const;

  double startTime() const;

  const std::string& name() const;

  const Mask& mask() const;

  const std::vector<Effect>& effects() const;

  double stretch() const;

  int64_t parentIndex() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Layer);
};

class ShapeLayer : public Layer {
 public:
  const std::vector<Shape> shapeItems() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ShapeLayer);
};

class SolidLayer : public Layer {
 public:
  const std::string& solidColor() const;

  double solidHeight() const;

  geom::Size solidSize() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(SolidLayer);
};

class PrecompLayer : public Layer {
 public:
  const std::string& referenceID() const;

  double timeRemapping() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(PrecompLayer);
};

class ImageLayer : public Layer {
 public:
  const std::string& referenceID() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ImageLayer);
};

class NullLayer : public Layer {
 public:
 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullLayer);
};

class TextLayer : public Layer {
 public:
  const TextData& textData() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TextLayer);
};

}  // namespace bodymovin
}  // namespace rl
