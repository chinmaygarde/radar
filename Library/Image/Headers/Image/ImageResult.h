// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Allocation.h>
#include <Core/Macros.h>
#include <Geometry/Size.h>

namespace rl {
namespace image {

class ImageResult {
 public:
  enum class Components {
    Invalid,
    Grey,
    GreyAlpha,
    RGB,
    RGBA,
  };

  ImageResult();

  ImageResult(geom::Size size,
              Components components,
              core::Allocation allocation);

  ImageResult(ImageResult&&);

  ImageResult& operator=(ImageResult&&);

  ~ImageResult();

  const geom::Size& size() const;

  bool wasSuccessful() const;

  Components components() const;

  const core::Allocation& allocation() const;

 private:
  bool _success;
  geom::Size _size;
  Components _components;
  core::Allocation _allocation;

  RL_DISALLOW_COPY_AND_ASSIGN(ImageResult);
};

}  // namespace image
}  // namespace rl
