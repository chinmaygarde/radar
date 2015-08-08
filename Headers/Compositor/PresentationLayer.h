// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONLAYER__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONLAYER__

#include "Interface/Layer.h"

namespace rl {
class PresentationLayer : public Layer {
 public:
  using Ref = std::shared_ptr<PresentationLayer>;

  PresentationLayer();

  /**
   *  The list of presentation sublayers of this layer.
   *
   *  @return the presentation sublayers
   */
  std::list<Ref> presentationSublayers() const;

  /**
   *  Renders the layer primitives in the specified frame
   *
   *  @param frame the frame to render the layer primitives into
   */
  void drawInFrame(Frame& frame);

 private:
  std::list<Ref> _presentationSublayers;
  std::unique_ptr<Primitive> _backgroundPrimitive;
  std::unique_ptr<Primitive> _foregroundPrimitive;

  DISALLOW_COPY_AND_ASSIGN(PresentationLayer);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PRESENTATIONLAYER__) */
