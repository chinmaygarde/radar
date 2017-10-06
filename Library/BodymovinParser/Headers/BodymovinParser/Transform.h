/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <BodymovinParser/Value.h>
#include <Core/Macros.h>
#include <memory>

namespace rl {
namespace bodymovin {

class Transform {
 public:
  Transform() = default;

  ~Transform() = default;

  const std::unique_ptr<ValueBase>& anchorPoint() const { return _anchorPoint; }

  const std::unique_ptr<ValueBase>& position() const { return _position; }

  const std::unique_ptr<ValueBase>& scale() const { return _scale; }

  const std::unique_ptr<ValueBase>& rotation() const { return _rotation; }

  const std::unique_ptr<ValueBase>& opacity() const { return _opacity; }

  const std::unique_ptr<ValueBase>& positionX() const { return _positionX; }

  const std::unique_ptr<ValueBase>& positionY() const { return _positionY; }

  void setAnchorPoint(std::unique_ptr<ValueBase> anchorPoint) {
    _anchorPoint = std::move(anchorPoint);
  }

  void setPosition(std::unique_ptr<ValueBase> position) {
    _position = std::move(position);
  }

  void setScale(std::unique_ptr<ValueBase> scale) { _scale = std::move(scale); }

  void setRotation(std::unique_ptr<ValueBase> rotation) {
    _rotation = std::move(rotation);
  }

  void setOpacity(std::unique_ptr<ValueBase> opacity) {
    _opacity = std::move(opacity);
  }

  void setPositionX(std::unique_ptr<ValueBase> positionX) {
    _positionX = std::move(positionX);
  }

  void setPositionY(std::unique_ptr<ValueBase> positionY) {
    _positionY = std::move(positionY);
  }

 private:
  std::unique_ptr<ValueBase> _anchorPoint;
  std::unique_ptr<ValueBase> _position;
  std::unique_ptr<ValueBase> _scale;
  std::unique_ptr<ValueBase> _rotation;
  std::unique_ptr<ValueBase> _opacity;
  std::unique_ptr<ValueBase> _positionX;
  std::unique_ptr<ValueBase> _positionY;

  RL_DISALLOW_COPY_AND_ASSIGN(Transform);
};

}  // namespace bodymovin
}  // namespace rl
