// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONLAYER__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONLAYER__

#include <Core/Core.h>
#include <Geometry/Matrix.h>
#include <Interface/Layer.h>

namespace rl {
class PresentationLayer {
 public:
  PresentationLayer(const Layer& layer);

 private:
  ~PresentationLayer();

  DISALLOW_COPY_AND_ASSIGN(PresentationLayer);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PRESENTATIONLAYER__) */
