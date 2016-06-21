// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_IMAGE_DATAIMAGESOURCE_H_
#define RADAR_IMAGE_DATAIMAGESOURCE_H_

#include <Core/Macros.h>
#include <Core/Allocation.h>

#include "ImageSource.h"

namespace rl {
namespace image {

class DataImageSource : public ImageSource {
 public:
  DataImageSource();

  DataImageSource(core::Allocation allocation);

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

 private:
  core::Allocation _allocation;

  ImageSource::Type type() const override;

  uint8_t* sourceData() const override;

  size_t sourceDataSize() const override;

  void onPrepareForUse() override;

  RL_DISALLOW_COPY_AND_ASSIGN(DataImageSource);
};

}  // namespace image
}  // namespace rl

#endif  // RADAR_IMAGE_DATAIMAGESOURCE_H_
