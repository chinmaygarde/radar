// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_COMPOSITOR_ENITITY__
#define __RADARLOVE_ANIMATION_COMPOSITOR_ENITITY__

#include <Core/Core.h>
#include "Geometry/Rect.h"
#include "Geometry/Matrix.h"

namespace rl {
class Entity {
 public:
  Entity();
  /**
   *  The frame specifies the origin and size of the entity in the coordinate
   *  space of its parent. This is a computed property derived from the bounds
   *  of the entity and its position.
   *
   *  @return the frame of the entity
   */
  Rect frame() const;

  /**
   *  Set the frame of the entity
   *
   *  @param frame the new frame
   */
  void setFrame(const Rect& frame);

  /**
   *  The bounds specifies the origin and size of the entity in its own
   *  coordinate space.
   *
   *  @return the bounds of the entity
   */
  const Rect& bounds() const;

  /**
   *  Set the bounds of the entity
   *
   *  @param bounds the new bounds
   */
  void setBounds(const Rect& bounds);

  /**
   *  The position specifies the coordinates of the anchor position of the
   *  entity relative to its parent
   *
   *  @return the position of the entity
   */
  const Point& position() const;

  /**
   *  Sets the position of the entity
   *
   *  @param point the new position
   */
  void setPosition(const Point& point);

  /**
   *  The position of the anchor point within this node in unit space
   *
   *  @return the anchor point
   */
  const Point& anchorPoint() const;

  /**
   *  Sets the new anchor point of this node
   *
   *  @param anchorPoint the new anchor point
   */
  void setAnchorPoint(const Point& anchorPoint);

  /**
   *  The transformation that is applied to the entity about its anchor point
   *
   *  @return the transformation applied to the node
   */
  const Matrix& transformation() const;

  /**
   *  Sets the transformation of the entity
   *
   *  @param transformation the new transformation
   */
  void setTransformation(const Matrix& transformation);

  /**
   *  The effective model matrix of the entity
   *
   *  @return the model matrix
   */
  const Matrix& modelMatrix();

 private:
  Rect _bounds;
  Point _position;
  Point _anchorPoint;
  Matrix _transformation;
  Matrix _modelMatrix;
  bool _modelMatrixDirty;

  DISALLOW_COPY_AND_ASSIGN(Entity);
};
}

#endif /* defined(__RADARLOVE_ANIMATION_COMPOSITOR_ENITITY__) */
