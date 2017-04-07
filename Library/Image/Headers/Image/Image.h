// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/File.h>
#include <Core/MessageSerializable.h>
#include <Geometry/Size.h>
#include <Image/ImageResult.h>

namespace rl {
namespace image {

class ImageSource;

class Image : public core::MessageSerializable {
 public:
  Image();

  Image(core::Allocation sourceAllocation);

  Image(core::FileHandle sourceFile);

  ~Image();

  ImageResult decode() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  bool isValid() const;

  struct Hash {
    std::size_t operator()(const Image& key) const;
  };

  struct Equal {
    bool operator()(const Image& lhs, const Image& rhs) const;
  };

 private:
  std::shared_ptr<ImageSource> _source;
};

}  // namespace image
}  // namespace rl
