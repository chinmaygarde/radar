// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Image/ImageEncoder.h>

#include <stb_image_write.h>

namespace rl {
namespace image {

ImageEncoder::ImageEncoder(Type type, core::URI uri)
    : ImageEncoder(type,
                   core::FileHandle{std::move(uri),
                                    core::FileHandle::AccessType::Write}) {}

ImageEncoder::ImageEncoder(Type type, core::FileHandle handle)
    : _isReady(false), _type(type), _handle(std::move(handle)) {
  if (!_handle.isValid()) {
    return;
  }

  _isReady = true;
}

ImageEncoder::~ImageEncoder() = default;

bool ImageEncoder::isReady() const {
  return _isReady;
}

bool ImageEncoder::encode(ImageResult image) {
  if (!_isReady) {
    return false;
  }

  if (!image.wasSuccessful()) {
    return false;
  }

  switch (_type) {
    case Type::PNG:
      return encodePNG(std::move(image));
  }

  return false;
}

int ComponentsToSize(ImageResult::Components components) {
  switch (components) {
    case ImageResult::Components::Invalid:
      return 0;
    case ImageResult::Components::Grey:
      return 1;
    case ImageResult::Components::GreyAlpha:
      return 2;
    case ImageResult::Components::RGB:
      return 3;
    case ImageResult::Components::RGBA:
      return 4;
  }

  return 0;
}

bool ImageEncoder::encodePNG(ImageResult image) {
  auto size = image.size();

  int componentSize = ComponentsToSize(image.components());

  auto callback = [](void* context, void* data, int size) {
    reinterpret_cast<ImageEncoder*>(context)->write(
        reinterpret_cast<const uint8_t*>(data), size);
  };

  return stbi_write_png_to_func(callback,                   //
                                this,                       //
                                size.width,                 //
                                size.height,                //
                                componentSize,              //
                                image.allocation().data(),  //
                                componentSize               //
                                ) == 1;
}

void ImageEncoder::write(const uint8_t* data, size_t size) {
  _handle.write(data, size);
}

}  // namespace image
}  // namespace rl
