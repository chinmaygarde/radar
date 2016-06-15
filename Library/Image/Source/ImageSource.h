// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_IMAGE_IMAGESOURCE_H_
#define RADAR_IMAGE_IMAGESOURCE_H_

#include <Core/Macros.h>
#include <Core/Allocation.h>
#include <Core/File.h>

namespace rl {
namespace image {

class ImageSource : public core::MessageSerializable {
 public:
  enum class Type : uint8_t {
    Unknown,
    File,
    Data,
  };

  static std::unique_ptr<ImageSource> Create(core::Allocation allocation);

  static std::unique_ptr<ImageSource> Create(core::FileHandle fileHandle);

  static std::shared_ptr<ImageSource> ImageSourceForType(Type type);

  ImageSource();

  virtual ~ImageSource();

 protected:
  friend class Image;

  virtual Type type() const = 0;

  virtual uint8_t* sourceData() const = 0;

  virtual size_t sourceDataSize() const = 0;
};

}  // namespace image
}  // namespace rl

#endif  // RADAR_IMAGE_IMAGESOURCE_H_
