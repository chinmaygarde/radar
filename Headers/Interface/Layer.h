// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_LAYER_
#define RADARLOVE_INTERFACE_LAYER_

#include <Core/Core.h>

#include <Interface/Entity.h>
#include <Coordinator/Color.h>
#include <Coordinator/Frame.h>
#include <Coordinator/Primitive.h>
#include <Coordinator/EntityArena.h>

#include <list>

namespace rl {
namespace interface {

class Layer : public Entity {
 public:
  using Ref = std::shared_ptr<Layer>;

  explicit Layer();

  ~Layer() override;

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

  void makeRootInInterface() const;

 private:
  std::list<Ref> _sublayers;
  Layer* _superlayer;

  RL_DISALLOW_COPY_AND_ASSIGN(Layer);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_LAYER_
