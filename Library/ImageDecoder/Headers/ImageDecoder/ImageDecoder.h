// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IMAGEDECODER_IMAGEDECODER_IMAGEDECODER_H_
#define IMAGEDECODER_IMAGEDECODER_IMAGEDECODER_H_

#include <Core/Core.h>
#include <Geometry/Size.h>

namespace rl {
namespace image {

enum class Components {
  Invalid,
  Grey,
  GreyAlpha,
  RGB,
  RGBA,
};

class ImageDecoderResult {
 public:
  ImageDecoderResult();

  ImageDecoderResult(geom::Size size,
                     Components components,
                     std::unique_ptr<core::Allocation> allocation);

  ImageDecoderResult(ImageDecoderResult&&);

  ~ImageDecoderResult();

  const geom::Size& size() const;

  bool wasSuccessful() const;

  Components components() const;

  const std::unique_ptr<core::Allocation>& allocation() const;

 private:
  bool _success;
  geom::Size _size;
  Components _components;
  std::unique_ptr<core::Allocation> _allocation;

  RL_DISALLOW_COPY_AND_ASSIGN(ImageDecoderResult);
};

class ImageDecoder {
 public:
  ImageDecoder(std::unique_ptr<core::Allocation> allocation);

  ImageDecoderResult decode() const;

  ~ImageDecoder();

 private:
  std::unique_ptr<core::Allocation> _allocation;

  RL_DISALLOW_COPY_AND_ASSIGN(ImageDecoder);
};

}  // namespace image
}  // namespace rl

#endif  // IMAGEDECODER_IMAGEDECODER_IMAGEDECODER_H_
