// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_LAYER__
#define __RADARLOVE_INTERFACE_LAYER__

#include <Core/Core.h>

#include "Compositor/Color.h"
#include "Compositor/Frame.h"
#include "Compositor/Primitive.h"
#include "Compositor/Entity.h"

#include <list>

namespace rl {
class Layer : public Entity {
 public:
  using Ref = std::shared_ptr<Layer>;

#pragma mark - Construction

  Layer();

  ~Layer();

#pragma mark - Layer Hierarchy
  /**
   *  Adds the layer as a sublayer of the current layer. If the layer already
   *  has a parent, it is removed from the same.
   */
  void addSublayer(Ref layer);

  /**
   *  Removes the layer from the collection of sublayers
   *
   *  @param layer the sublayer to remove
   */
  void removeSublayer(Ref layer);

#pragma mark - Display Properties

  /**
   *  The background color of the layer
   *
   *  @return the background color
   */
  const Color& backgroundColor() const;

  /**
   *  Set the new background color of the layer
   *
   *  @param backgroundColor the new background color
   */
  void setBackgroundColor(const Color& backgroundColor);

  /**
   *  The opacity of the layer. 0.0 is fully transparent and 1.0 is fully
   *  opaque. Default it 1.0.
   *
   *  @return the opacity of the layer
   */
  double opacity() const;

  /**
   *  Set the new opacity of the layer
   *
   *  @param opacity the new opacity
   */
  void setOpacity(double opacity);

  /**
   *  Renders the layer primitives in the specified frame
   *
   *  @param frame the frame to render the layer primitives into
   */
  void drawInFrame(Frame& frame);

 private:
  Color _backgroundColor;
  double _opacity;
  std::list<Ref> _sublayers;
  Layer* _superlayer;
  std::unique_ptr<Primitive> _backgroundPrimitive;
  std::unique_ptr<Primitive> _foregroundPrimitive;

  DISALLOW_COPY_AND_ASSIGN(Layer);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_LAYER__) */
