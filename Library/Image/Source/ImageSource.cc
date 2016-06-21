// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ImageSource.h"
#include "DataImageSource.h"
#include "FileImageSource.h"

namespace rl {
namespace image {

std::unique_ptr<ImageSource> ImageSource::Create(core::Allocation allocation) {
  return core::make_unique<DataImageSource>(std::move(allocation));
}

std::unique_ptr<ImageSource> ImageSource::Create(core::FileHandle fileHandle) {
  return core::make_unique<FileImageSource>(std::move(fileHandle));
}

std::shared_ptr<ImageSource> ImageSource::ImageSourceForType(Type type) {
  switch (type) {
    case Type::File:
      return std::make_shared<FileImageSource>();
    case Type::Data:
      return std::make_shared<DataImageSource>();
    default:
      return nullptr;
  }

  return nullptr;
}

ImageSource::ImageSource() = default;

ImageSource::~ImageSource() = default;

void ImageSource::prepareForUse() {
  if (_prepared) {
    return;
  }

  onPrepareForUse();

  _prepared = true;
}

}  // namespace image
}  // namespace rl
