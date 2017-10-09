/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BodymovinParser/Animation.h>
#include <rapidjson/document.h>
#include <limits>
#include "JSONHelper.h"

namespace rl {
namespace bodymovin {

std::unique_ptr<Animation> Animation::ParseManifest(
    const core::FileMapping& mapping) {
  rapidjson::Document document;
  document.Parse(reinterpret_cast<const char*>(mapping.mapping()),
                 mapping.size());

  if (document.HasParseError()) {
    return nullptr;
  }

  auto animation = std::make_unique<Animation>();

  if (!ReadMember(document, *animation)) {
    return nullptr;
  }

  return animation;
}

}  // namespace bodymovin
}  // namespace rl
