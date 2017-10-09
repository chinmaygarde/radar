/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <BodymovinParser/Asset.h>
#include <BodymovinParser/Character.h>
#include <BodymovinParser/Layer.h>
#include <Core/FileMapping.h>
#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <memory>
#include <string>

namespace rl {
namespace bodymovin {

class Animation {
 public:
  static std::unique_ptr<Animation> ParseManifest(
      const core::FileMapping& mapping);

  Animation() = default;

  ~Animation() = default;

  double inPoint() const { return _inPoint; }

  double outPoint() const { return _outPoint; }

  double frameRate() const { return _frameRate; }

  geom::Size compositionSize() const { return _compositionSize; }

  const std::string& bodymovinVersion() const { return _bodymovinVersion; }

  const std::vector<std::unique_ptr<Layer>>& layers() const { return _layers; }

  const std::vector<Asset>& assets() const { return _assets; }

  const std::vector<Character>& characters() const { return _characters; }

  void setInPoint(double inPoint) { _inPoint = inPoint; }

  void setOutPoint(double outPoint) { _outPoint = outPoint; }

  void setFrameRate(double frameRate) { _frameRate = frameRate; }

  void setCompositionSize(geom::Size compositionSize) {
    _compositionSize = compositionSize;
  }

  void setBodymovinVersion(std::string bodymovinVersion) {
    _bodymovinVersion = std::move(bodymovinVersion);
  }

  void setLayers(std::vector<std::unique_ptr<Layer>> layers) {
    _layers = std::move(layers);
  }

  void setAssets(std::vector<Asset> assets) { _assets = std::move(assets); }

  void setCharacters(std::vector<Character> characters) {
    _characters = std::move(characters);
  }

 private:
  double _inPoint = 0.0;
  double _outPoint = 0.0;
  double _frameRate = 0.0;
  geom::Size _compositionSize;
  std::string _bodymovinVersion;
  std::vector<std::unique_ptr<Layer>> _layers;
  std::vector<Asset> _assets;
  std::vector<Character> _characters;

  RL_DISALLOW_COPY_AND_ASSIGN(Animation);
};

}  // namespace bodymovin
}  // namespace rl
