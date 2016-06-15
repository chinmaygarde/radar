// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Image/Image.h>

#include <stb_image.h>

#include "ImageSource.h"

namespace rl {
namespace image {

Image::Image() = default;

Image::Image(core::Allocation sourceAllocation)
    : _source(ImageSource::Create(std::move(sourceAllocation))) {}

Image::Image(core::FileHandle sourceFile)
    : _source(ImageSource::Create(std::move(sourceFile))) {}

bool Image::serialize(core::Message& message) const {
  if (_source == nullptr) {
    return false;
  }

  if (!message.encode(_source->type())) {
    return false;
  }

  return _source->serialize(message);
}

bool Image::deserialize(core::Message& message, core::Namespace* ns) {
  if (_source == nullptr) {
    return false;
  }

  auto type = ImageSource::Type::Unknown;

  if (!message.decode(type, ns)) {
    return false;
  }

  auto source = ImageSource::ImageSourceForType(type);

  if (source == nullptr) {
    return false;
  }

  if (source->deserialize(message, ns)) {
    _source = source;
  } else {
    return false;
  }

  return false;
}

ImageResult Image::decode() const {
  if (_source == nullptr || _source->sourceDataSize() == 0) {
    return {};
  }

  int width = 0;
  int height = 0;
  int comps = 0;

  stbi_uc* decoded =
      stbi_load_from_memory(_source->sourceData(),      // Source Data
                            _source->sourceDataSize(),  // Source Data Size
                            &width,                     // Out: Width
                            &height,                    // Out: Height
                            &comps,                     // Out: Components
                            STBI_default);

  auto destinationAllocation =
      core::Allocation{decoded, width * height * comps * sizeof(stbi_uc)};

  /*
   *  If either the decoded allocation is null or the size works out to be zero,
   *  the allocation will mark itself as not ready and we know that the decode
   *  job failed.
   */

  if (!destinationAllocation.isReady()) {
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

Image::~Image() = default;

}  // namespace image
}  // namespace rl
