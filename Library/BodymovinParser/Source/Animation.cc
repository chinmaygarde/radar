/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BodymovinParser/Animation.h>

namespace rl {
namespace bodymovin {

std::unique_ptr<Animation> Animation::Create(const core::FileMapping& mapping) {
  if (mapping.size() == 0) {
    return nullptr;
  }

  return nullptr;
}

Animation::Animation() = default;

Animation::~Animation() = default;

}  // namespace bodymovin
}  // namespace rl
