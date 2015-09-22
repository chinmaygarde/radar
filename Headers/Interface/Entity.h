// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_ENTITY__
#define __RADARLOVE_INTERFACE_ENTITY__

#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Compositor/Color.h>

namespace rl {

class GestureRecognizer;

class Entity {
 public:
  // clang-format off
  enum Property {
    None            = 0,
    Created         = 1 << 0,
    Destroyed       = 1 << 1,
    LifeCycle       = Created | Destroyed,
    AddedTo         = 1 << 2,
    RemovedFrom     = 1 << 3,
    Hierarchy       = AddedTo | RemovedFrom,
    Bounds          = 1 << 4,
    Position        = 1 << 5,
    AnchorPoint     = 1 << 6,
    Transformation  = 1 << 7,
    BackgroundColor = 1 << 8,
    Opacity         = 1 << 9,
    MakeRoot        = 1 << 10,
  };
  // clang-format on

  template <typename T>
  struct Accessors {
    using Getter = std::function<const T&(Entity&)>;
    Getter getter;
    using Setter = std::function<void(Entity&, const T&)>;
    Setter setter;
  };

#pragma mark - Managing Entity Lifecycle

  /**
   *  Construct a default entity
   */
  explicit Entity(bool notifiesInterfaceOnUpdate);

  ~Entity();

#pragma mark - Entity Identification
  using Identifier = uint64_t;
  static const Identifier IdentifierNone = 0;
  Identifier identifier() const;

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
  const double& opacity() const;

  /**
   *  Set the new opacity of the entity
   *
   *  @param opacity the new opacity
   */
  void setOpacity(double opacity);

#pragma mark - Managing Gesture Recognition

  void setupGestureRecognizer(GestureRecognizer&& recognizer);

#pragma mark - Managing Entity Update

  void merge(const Entity& entity);

 protected:
  explicit Entity(const Entity& entity);
  explicit Entity(Identifier identifier);

  void notifyInterfaceIfNecessary(Property property,
                                  Identifier other = 0) const;

 private:
  Identifier _identifier;
  Rect _bounds;
  Point _position;
  Point _anchorPoint;
  Matrix _transformation;
  Color _backgroundColor;
  double _opacity;
  bool _notifiesInterfaceOnUpdate;

  RL_DISALLOW_COPY_AND_ASSIGN(Entity);
};

extern const Entity::Accessors<Rect> BoundsAccessors;
extern const Entity::Accessors<Point> PositionAccessors;
extern const Entity::Accessors<Point> AnchorPointAccessors;
extern const Entity::Accessors<Matrix> TransformationAccessors;
extern const Entity::Accessors<Color> BackgroundColorAccessors;
extern const Entity::Accessors<double> OpacityAccessors;

}  // namespace rl

#endif /* defined(__RADARLOVE_INTERFACE_ENTITY__) */
