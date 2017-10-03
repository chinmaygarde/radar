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
  static std::unique_ptr<Animation> Create(const core::FileMapping& mapping);

  ~Animation();

  double inPoint() const;

  double outPoint() const;

  double frameRate() const;

  geom::Size compositionSize() const;

  const std::string& bodymovinVersion() const;

  const std::vector<Layer> layers() const;

  const std::vector<Asset> assets() const;

  const std::vector<Character> characters() const;

 private:
  Animation();

  RL_DISALLOW_COPY_AND_ASSIGN(Animation);
};

}  // namespace bodymovin
}  // namespace rl
