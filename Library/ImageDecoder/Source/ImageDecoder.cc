// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ImageDecoder/ImageDecoder.h>

#include <stb_image.h>

namespace rl {
namespace image {

ImageDecoder::ImageDecoder(std::unique_ptr<core::Allocation> sourceAllocation)
    : _sourceAllocation(std::move(sourceAllocation)) {}

ImageResult ImageDecoder::decode() const {
  if (_sourceAllocation == nullptr || _sourceAllocation->size() == 0) {
    return {};
  }

  int width = 0;
  int height = 0;
  int comps = 0;

  stbi_uc* decoded = stbi_load_from_memory(_sourceAllocation->data(),
                                           _sourceAllocation->size(), &width,
                                           &height, &comps, STBI_default);

  auto destinationAllocation = core::make_unique<core::Allocation>(
      decoded, width * height * comps * sizeof(stbi_uc));

  /*
   *  If either the decoded allocation is null or the size works out to be zero,
   *  the allocation will mark itself as not ready and we know that the decode
   *  job failed.
   */

  if (!destinationAllocation->isReady()) {
    return {};
  }

  /*
   *  Make sure we got a valid component set.
   */
  auto components = Components::Invalid;

  switch (comps) {
    case STBI_grey:
      components = Components::Grey;
      break;
    case STBI_grey_alpha:
      components = Components::GreyAlpha;
      break;
    case STBI_rgb:
      components = Components::RGB;
      break;
    case STBI_rgb_alpha:
      components = Components::RGBA;
      break;
    default:
      components = Components::Invalid;
      break;
  }

  if (components == Components::Invalid) {
    return {};
  }

  return ImageResult{
      geom::Size{static_cast<double>(width),
                 static_cast<double>(height)},  // size
      components,                               // components
      std::move(destinationAllocation)          // allocation

  };
}

ImageDecoder::~ImageDecoder() {}

}  // namespace image
}  // namespace rl
