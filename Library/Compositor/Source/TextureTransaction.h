// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_TEXTURETRANSACTION_H_
#define RADAR_COMPOSITOR_TEXTURETRANSACTION_H_

#include <Core/Macros.h>
#include <Core/WorkQueue.h>
#include <Core/Mutexed.h>

#include <Image/Image.h>

#include "Texture.h"

#include <unordered_map>

namespace rl {
namespace compositor {

class TextureTransaction {
 public:
  TextureTransaction();

  ~TextureTransaction();

  bool addImage(image::Image image);

  bool commit(core::WorkQueue& workqueue);

 private:
  using GuardedResults = core::Mutexed<std::unordered_map<image::Image,
                                                          Texture,
                                                          image::Image::Hash,
                                                          image::Image::Equal>>;
  using GuardedImagesSet =
      core::Mutexed<std::unordered_set<image::Image,
                                       image::Image::Hash,
                                       image::Image::Equal>>;

  GuardedImagesSet _images;
  GuardedResults _results;
  std::unique_ptr<core::Latch> _resultsLatch;

  bool uncompressImages(core::WorkQueue& workqueue);

  bool uploadImagesToVRAM();
  void uncompressOnWQ(image::Image image);

  RL_DISALLOW_COPY_AND_ASSIGN(TextureTransaction);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTURETRANSACTION_H_
