// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_IMAGE_IMAGEENCODER_H_
#define RADAR_IMAGE_IMAGEENCODER_H_

#include <Core/Macros.h>
#include <Image/ImageResult.h>
#include <Core/FileIOAdapter.h>
#include <Geometry/Size.h>

namespace rl {
namespace image {

class ImageEncoder {
 public:
  enum class Type {
    PNG,
  };

  ImageEncoder(Type type, core::URI uri);

  ImageEncoder(Type type, core::FileHandle handle);

  ~ImageEncoder();

  bool isReady() const;

  bool encode(ImageResult image);

 private:
  bool _isReady;
  Type _type;
  core::FileIOAdapter _adapter;

  bool encodePNG(ImageResult image);

  void write(const uint8_t* data, size_t size);

  RL_DISALLOW_COPY_AND_ASSIGN(ImageEncoder);
};

}  // namespace image
}  // namespace rl

#endif  // RADAR_IMAGE_IMAGEENCODER_H_
