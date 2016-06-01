// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IMAGEDECODER_IMAGEDECODER_IMAGEDECODER_H_
#define IMAGEDECODER_IMAGEDECODER_IMAGEDECODER_H_

#include <Core/Core.h>
#include <Geometry/Size.h>
#include <ImageDecoder/ImageResult.h>

namespace rl {
namespace image {

class ImageDecoder {
 public:
  ImageDecoder(std::unique_ptr<core::Allocation> sourceAllocation);

  ImageResult decode() const;

  ~ImageDecoder();

 private:
  std::unique_ptr<core::Allocation> _sourceAllocation;

  RL_DISALLOW_COPY_AND_ASSIGN(ImageDecoder);
};

}  // namespace image
}  // namespace rl

#endif  // IMAGEDECODER_IMAGEDECODER_IMAGEDECODER_H_
