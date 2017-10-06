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
#include <limits>
#include <string>
#include <vector>

namespace rl {
namespace bodymovin {

class Layer {
 public:
  Layer() {}

  virtual ~Layer() {}

  const std::unique_ptr<Transform>& transform() const { return _transform; }

  bool shouldAutoOrient() const { return _shouldAutoOrient; }

  BlendMode blendMode() const { return _blendMode; }

  bool is3DLayer() const { return _is3DLayer; }

  int64_t index() const { return _index; }

  const std::string& className() const { return _className; }

  const std::string& layerHtmlID() const { return _layerHtmlID; }

  double inPoint() const { return _inPoint; }

  double outPoint() const { return _outPoint; }

  double startTime() const { return _startTime; }

  const std::string& name() const { return _name; }

  const std::unique_ptr<Mask>& mask() const { return _mask; }

  const std::vector<std::unique_ptr<Effect>>& effects() const {
    return _effects;
  }

  double stretch() const { return _stretch; }

  int64_t parentIndex() const { return _parentIndex; }

  void setTransform(std::unique_ptr<Transform> transform) {
    _transform = std::move(transform);
  }

  void setShouldAutoOrient(bool shouldAutoOrient) {
    _shouldAutoOrient = shouldAutoOrient;
  }

  void setBlendMode(BlendMode blendMode) { _blendMode = blendMode; }

  void setIs3DLayer(bool is3DLayer) { _is3DLayer = is3DLayer; }

  void setIndex(int64_t index) { _index = index; }

  void setClassName(std::string className) {
    _className = std::move(className);
  }

  void setLayerHtmlID(std::string layerHtmlID) {
    _layerHtmlID = std::move(layerHtmlID);
  }

  void setInPoint(double inPoint) { _inPoint = inPoint; }

  void setOutPoint(double outPoint) { _outPoint = outPoint; }

  void setStartTime(double startTime) { _startTime = startTime; }

  void setName(std::string name) { _name = std::move(name); }

  void setMask(std::unique_ptr<Mask> mask) { _mask = std::move(mask); }

  void setEffects(std::vector<std::unique_ptr<Effect>> effects) {
    _effects = std::move(effects);
  }

  void setStretch(double stretch) { _stretch = stretch; }

  void setParentIndex(int64_t parentIndex) { _parentIndex = parentIndex; }

 private:
  std::unique_ptr<Transform> _transform;
  bool _shouldAutoOrient = false;
  BlendMode _blendMode = BlendMode::Normal;
  bool _is3DLayer = false;
  int64_t _index = std::numeric_limits<int64_t>::max();
  std::string _className;
  std::string _layerHtmlID;
  double _inPoint = 0.0;
  double _outPoint = 0.0;
  double _startTime = 0.0;
  std::string _name;
  std::unique_ptr<Mask> _mask;
  std::vector<std::unique_ptr<Effect>> _effects;
  double _stretch = 0.0;
  int64_t _parentIndex = std::numeric_limits<int64_t>::max();

  RL_DISALLOW_COPY_AND_ASSIGN(Layer);
};

class ShapeLayer : public Layer {
 public:
  ShapeLayer() {}

  ~ShapeLayer() override {}

  const std::vector<std::unique_ptr<Shape>>& shapeItems() const {
    return _shapeItems;
  }

  void setShapeItems(std::vector<std::unique_ptr<Shape>> shapeItems) {
    _shapeItems = std::move(shapeItems);
  }

 private:
  std::vector<std::unique_ptr<Shape>> _shapeItems;

  RL_DISALLOW_COPY_AND_ASSIGN(ShapeLayer);
};

class SolidLayer : public Layer {
 public:
  SolidLayer() {}

  ~SolidLayer() override {}

  const std::string& solidColor() const { return _solidColor; }

  geom::Size solidSize() const { return _solidSize; }

  void setSolidColor(std::string solidColor) { _solidColor = solidColor; }

  void setSolidSize(const geom::Size& solidSize) { _solidSize = solidSize; }

 private:
  std::string _solidColor;
  geom::Size _solidSize;

  RL_DISALLOW_COPY_AND_ASSIGN(SolidLayer);
};

class PrecompLayer : public Layer {
 public:
  PrecompLayer() {}

  ~PrecompLayer() override {}

  const std::string& referenceID() const { return _referenceID; }

  void setReferenceID(std::string referenceID) {
    _referenceID = std::move(referenceID);
  }

  double timeRemapping() const { return _timeRemapping; }

  void setTimeRemapping(double timeRemapping) {
    _timeRemapping = timeRemapping;
  }

 private:
  std::string _referenceID;
  double _timeRemapping = 0.0;
  RL_DISALLOW_COPY_AND_ASSIGN(PrecompLayer);
};

class ImageLayer : public Layer {
 public:
  ImageLayer() {}

  ~ImageLayer() override {}

  const std::string& referenceID() const { return _referenceID; }

  void setReferenceID(std::string referenceID) {
    _referenceID = std::move(referenceID);
  }

 private:
  std::string _referenceID;

  RL_DISALLOW_COPY_AND_ASSIGN(ImageLayer);
};

class NullLayer : public Layer {
 public:
  NullLayer() {}

  ~NullLayer() override {}

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullLayer);
};

class TextLayer : public Layer {
 public:
  TextLayer() {}

  ~TextLayer() override {}

  const std::unique_ptr<TextData>& textData() const { return _textData; }

  void setTextData(std::unique_ptr<TextData> textData) {
    _textData = std::move(textData);
  }

 private:
  std::unique_ptr<TextData> _textData;

  RL_DISALLOW_COPY_AND_ASSIGN(TextLayer);
};

}  // namespace bodymovin
}  // namespace rl
