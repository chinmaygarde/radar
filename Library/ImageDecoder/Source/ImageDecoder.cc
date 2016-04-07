// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ImageDecoder/ImageDecoder.h>

#include <stb_image.h>

namespace rl {
namespace image {

ImageDecoderResult::ImageDecoderResult(
    geom::Size size,
    Components components,
    std::unique_ptr<core::Allocation> allocation)
    : _success(true),
      _size(size),
      _components(components),
      _allocation(std::move(allocation)) {}

ImageDecoderResult::ImageDecoderResult() : _success(false) {}

ImageDecoderResult::ImageDecoderResult(ImageDecoderResult&&) = default;

bool ImageDecoderResult::wasSuccessful() const {
  return _success;
}

const geom::Size& ImageDecoderResult::size() const {
  return _size;
}

Components ImageDecoderResult::components() const {
  return _components;
}

const std::unique_ptr<core::Allocation>& ImageDecoderResult::allocation()
    const {
  return _allocation;
}

ImageDecoderResult::~ImageDecoderResult() {}

ImageDecoder::ImageDecoder(std::unique_ptr<core::Allocation> allocation)
    : _allocation(std::move(allocation)) {}

ImageDecoderResult ImageDecoder::decode() const {
  if (_allocation == nullptr || _allocation->size() == 0) {
    return {};
  }

  int width = 0;
  int height = 0;
  int components = 0;

  stbi_uc* decoded =
      stbi_load_from_memory(_allocation->data(), _allocation->size(), &width,
                            &height, &components, STBI_default);

  auto allocation = core::make_unique<core::Allocation>(
      decoded, width * height * components * sizeof(stbi_uc));

  /*
   *  If either the decoded allocation is null or the size works out to be zero,
   *  the allocation will mark itself as not ready and we know that the decode
   *  job failed.
   */

  if (!allocation->isReady()) {
    return {};
  }

  /*
   *  Make sure we got a valid component set.
   */
  auto comp = Components::Invalid;

  switch (components) {
    case STBI_grey:
      comp = Components::Grey;
      break;
    case STBI_grey_alpha:
      comp = Components::GreyAlpha;
      break;
    case STBI_rgb:
      comp = Components::RGB;
      break;
    case STBI_rgb_alpha:
      comp = Components::RGBA;
      break;
    default:
      comp = Components::Invalid;
      break;
  }

  if (comp == Components::Invalid) {
    return {};
  }

  return ImageDecoderResult{
      geom::Size{static_cast<double>(width),
                 static_cast<double>(height)},  // size
      comp,                                     // components
      std::move(allocation)                     // allocation

  };
}

ImageDecoder::~ImageDecoder() {}

}  // namespace image
}  // namespace rl
