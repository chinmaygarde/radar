// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_ENTITY__
#define __RADARLOVE_INTERFACE_ENTITY__

#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Compositor/Color.h>

namespace rl {
class Entity {
 public:
  // clang-format off
  enum Property {
    Bounds          = 1 << 0,
    Position        = 1 << 1,
    AnchorPoint     = 1 << 2,
    Transformation  = 1 << 3,
    BackgroundColor = 1 << 4,
    Opacity         = 1 << 5,
    Created         = 1 << 6,
    Destroyed       = 1 << 7,
  };
// clang-format on
#pragma mark - Managing Entity Lifecycle

  /**
   *  Construct a default entity
   */
  explicit Entity(bool notifiesInterfaceOnUpdate);

  ~Entity();

#pragma mark - Entity Identification
  using Identifer = uint64_t;
  Identifer identifier() const;

#pragma mark - Geometry

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
  Matrix modelMatrix() const;

#pragma mark - Display Properties

  /**
   *  The background color of the entity
   *
   *  @return the background color
   */
  const Color& backgroundColor() const;

  /**
   *  Set the new background color of the entity
   *
   *  @param backgroundColor the new background color
   */
  void setBackgroundColor(const Color& backgroundColor);

  /**
   *  The opacity of the entity. 0.0 is fully transparent and 1.0 is fully
   *  opaque. Default it 1.0.
   *
   *  @return the opacity of the entity
   */
  double opacity() const;

  /**
   *  Set the new opacity of the entity
   *
   *  @param opacity the new opacity
   */
  void setOpacity(double opacity);

#pragma mark - Managing Entity Update

  void merge(const Entity& entity);

 protected:
  explicit Entity(const Entity& entity);

 private:
  Identifer _identifier;
  Rect _bounds;
  Point _position;
  Point _anchorPoint;
  Matrix _transformation;
  Color _backgroundColor;
  double _opacity;
  bool _notifiesInterfaceOnUpdate;

  void notifyInterfaceIfNecessary(Property property);

  DISALLOW_COPY_AND_ASSIGN(Entity);
};

}  // namespace rl

#endif /* defined(__RADARLOVE_INTERFACE_ENTITY__) */
