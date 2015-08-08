// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_LAYER__
#define __RADARLOVE_INTERFACE_LAYER__

#include <Core/Core.h>

#include <Geometry/Rect.h>
#include <Compositor/Color.h>
#include <Compositor/Frame.h>
#include <Compositor/Primitive.h>

#include <list>

namespace rl {
class Layer {
 public:
  using Ref = std::shared_ptr<Layer>;

#pragma mark - Construction

  Layer();

  ~Layer();

#pragma mark - Layer Geometry

  /**
   *  The frame specifies the origin and size of the layer in the coordinate
   *  space of its parent. This is a computed property derived from the bounds
   *  of the layer and its position.
   *
   *  @return the frame of the layer
   */
  Rect frame() const;

  /**
   *  Set the frame of the layer
   *
   *  @param frame the new frame
   */
  void setFrame(const Rect& frame);

  /**
   *  The bounds specifies the origin and size of the layer in its own
   *  coordinate space.
   *
   *  @return the bounds of the layer
   */
  const Rect& bounds() const;

  /**
   *  Set the bounds of the layer
   *
   *  @param bounds the new bounds
   */
  void setBounds(const Rect& bounds);

  /**
   *  The position specifies the coordinates of the anchor position of the
   *  layer relative to its parent
   *
   *  @return the position of the layer
   */
  const Point& position() const;

  /**
   *  Sets the position of the layer
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
   *  The transformation that is applied to the layer about its anchor point
   *
   *  @return the transformation applied to the node
   */
  const Matrix& transformation() const;

  /**
   *  Sets the transformation of the layer
   *
   *  @param transformation the new transformation
   */
  void setTransformation(const Matrix& transformation);

  /**
   *  The effective model matrix of the layer
   *
   *  @return the model matrix
   */
  const Matrix& modelMatrix();

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
  std::list<Ref> sublayers() const;

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

 private:
  Rect _bounds;
  Point _position;
  Point _anchorPoint;
  Matrix _transformation;
  Matrix _modelMatrix;
  bool _modelMatrixDirty;
  Color _backgroundColor;
  double _opacity;
  std::list<Ref> _sublayers;
  Layer* _superlayer;

  DISALLOW_COPY_AND_ASSIGN(Layer);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_LAYER__) */
