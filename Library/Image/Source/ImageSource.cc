// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ImageSource.h"
#include "DataImageSource.h"
#include "FileImageSource.h"

namespace rl {
namespace image {

std::unique_ptr<ImageSource> ImageSource::Create(
    std::unique_ptr<core::Allocation> allocation) {
  return core::make_unique<DataImageSource>(std::move(allocation));
}

std::unique_ptr<ImageSource> ImageSource::Create(
    std::unique_ptr<core::File> file) {
  return core::make_unique<FileImageSource>(std::move(file));
}

ImageSource::ImageSource() = default;

ImageSource::~ImageSource() = default;

}  // namespace image
}  // namespace rl
