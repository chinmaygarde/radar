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
  Animation(const core::FileMapping& mapping);

  ~Animation();

  bool isValid() const;

  double inPoint() const;

  double outPoint() const;

  double frameRate() const;

  geom::Size compositionSize() const;

  const std::string& bodymovinVersion() const;

  const std::vector<std::unique_ptr<Layer>>& layers() const;

  const std::vector<Asset>& assets() const;

  const std::vector<Character>& characters() const;

 private:
  bool _valid = false;
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
