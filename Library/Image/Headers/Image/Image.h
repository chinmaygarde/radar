// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_IMAGE_IMAGE_H_
#define RADAR_IMAGE_IMAGE_H_

#include <Core/Core.h>
#include <Geometry/Size.h>
#include <Image/ImageResult.h>

namespace rl {
namespace image {

class ImageSource;

class Image {
 public:
  Image(std::unique_ptr<core::Allocation> sourceAllocation);

  Image(std::unique_ptr<core::File> sourceFile);

  ImageResult decode() const;

  ~Image();

 private:
  std::unique_ptr<ImageSource> _source;

  RL_DISALLOW_COPY_AND_ASSIGN(Image);
};

}  // namespace image
}  // namespace rl

#endif  // RADAR_IMAGE_IMAGE_H_
