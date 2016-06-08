// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_IMAGE_FILEIMAGESOURCE_H_
#define RADAR_IMAGE_FILEIMAGESOURCE_H_

#include <Core/Macros.h>
#include <Core/FileMapping.h>

#include "ImageSource.h"

namespace rl {
namespace image {

class FileImageSource : public ImageSource {
 public:
  FileImageSource();

  FileImageSource(const core::File& file);

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

 private:
  core::FileMapping _mapping;

  ImageSource::Type type() const override;

  uint8_t* sourceData() const override;

  size_t sourceDataSize() const override;

  RL_DISALLOW_COPY_AND_ASSIGN(FileImageSource);
};

}  // namespace image
}  // namespace rl

#endif  // RADAR_IMAGE_FILEIMAGESOURCE_H_
