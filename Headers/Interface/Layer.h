// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_LAYER__
#define __RADARLOVE_INTERFACE_LAYER__

#include <Core/Core.h>

#include <Interface/Entity.h>
#include <Compositor/Color.h>
#include <Compositor/Frame.h>
#include <Compositor/Primitive.h>
#include <Compositor/EntityArena.h>

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

  /**
   *  The list of references to the sublayers of this layer
   *
   *  @return the sublayers of this layer
   */
  const std::list<Ref>& sublayers() const;

  /**
   *  Return the current superlayer of the layer
   *
   *  @return the superlayer
   */
  const Layer* superlayer() const;

 private:
  std::list<Ref> _sublayers;
  Layer* _superlayer;

  DISALLOW_COPY_AND_ASSIGN(Layer);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_LAYER__) */
