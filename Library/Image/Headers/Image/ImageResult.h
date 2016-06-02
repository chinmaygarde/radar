// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_IMAGE_IMAGERESULT_H_
#define RADAR_IMAGE_IMAGERESULT_H_

#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <Core/Allocation.h>

namespace rl {
namespace image {

enum class Components {
  Invalid,
  Grey,
  GreyAlpha,
  RGB,
  RGBA,
};

class ImageResult {
 public:
  ImageResult();

  ImageResult(geom::Size size,
              Components components,
              std::unique_ptr<core::Allocation> allocation);

  ImageResult(ImageResult&&);

  ~ImageResult();

  const geom::Size& size() const;

  bool wasSuccessful() const;

  Components components() const;

  const std::unique_ptr<core::Allocation>& allocation() const;

 private:
  bool _success;
  geom::Size _size;
  Components _components;
  std::unique_ptr<core::Allocation> _allocation;

  RL_DISALLOW_COPY_AND_ASSIGN(ImageResult);
};

}  // namespace image
}  // namespace rl

#endif  // RADAR_IMAGE_IMAGERESULT_H_
