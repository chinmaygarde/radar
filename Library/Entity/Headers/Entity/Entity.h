// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_ENTITY_
#define RADARLOVE_INTERFACE_ENTITY_

#include <Entity/Color.h>
#include <Core/Core.h>
#include <Geometry/Geometry.h>
#include <Image/Image.h>

namespace rl {
namespace entity {

class Entity : public core::ArchiveSerializable {
 public:
  using PropertyMaskType = uint16_t;

  enum class Property : PropertyMaskType {
    None,

    AddedTo,
    RemovedFrom,
    Bounds,
    Position,
    AnchorPoint,
    Transformation,
    BackgroundColor,
    Contents,
    Opacity,
    MakeRoot,

    Sentinel,
  };

#define RL_MASK(x) 1 << static_cast<uint32_t>(Property::x)
  // clang-format off
  enum PropertyMask {
    AddedToMask         = RL_MASK(AddedTo),
    RemovedFromMask     = RL_MASK(RemovedFrom),
    BoundsMask          = RL_MASK(Bounds),
    PositionMask        = RL_MASK(Position),
    AnchorPointMask     = RL_MASK(AnchorPoint),
    TransformationMask  = RL_MASK(Transformation),
    BackgroundColorMask = RL_MASK(BackgroundColor),
    ContentsMask        = RL_MASK(Contents),
    OpacityMask         = RL_MASK(Opacity),
    MakeRootMask        = RL_MASK(MakeRoot),
  };
// clang-format on
#undef RL_MASK

  using UpdateCallback = std::function<void(const Entity& /*entity*/,
                                            Entity::Property /*property*/,
                                            core::Name /*otherIdentifier*/)>;

  Entity(core::Name identifier, UpdateCallback updateCallback = nullptr);

  Entity(Entity&& entity);

  virtual ~Entity();

  core::Name identifier() const;

  /**
   *  The frame specifies the origin and size of the entity in the coordinate
   *  space of its parent. This is a computed property derived from the bounds
   *  of the entity and its position.
   *
   *  @return the frame of the entity
   */
  geom::Rect frame() const;

  /**
   *  Set the frame of the entity
   *
   *  @param frame the new frame
   */
  void setFrame(const geom::Rect& frame);

  /**
   *  The bounds specifies the origin and size of the entity in its own
   *  coordinate space.
   *
   *  @return the bounds of the entity
   */
  const geom::Rect& bounds() const;

  /**
   *  Set the bounds of the entity
   *
   *  @param bounds the new bounds
   */
  void setBounds(const geom::Rect& bounds);

  /**
   *  The position specifies the coordinates of the anchor position of the
   *  entity relative to its parent
   *
   *  @return the position of the entity
   */
  const geom::Point& position() const;

  /**
   *  Sets the position of the entity
   *
   *  @param point the new position
   */
  void setPosition(const geom::Point& point);

  /**
   *  The position of the anchor point within this node in unit space
   *
   *  @return the anchor point
   */
  const geom::Point& anchorPoint() const;

  /**
   *  Sets the new anchor point of this node
   *
   *  @param anchorPoint the new anchor point
   */
  void setAnchorPoint(const geom::Point& anchorPoint);

  /**
   *  The transformation that is applied to the entity about its anchor point
   *
   *  @return the transformation applied to the node
   */
  const geom::Matrix& transformation() const;

  /**
   *  Sets the transformation of the entity
   *
   *  @param transformation the new transformation
   */
  void setTransformation(const geom::Matrix& transformation);

  /**
   *  The model matrix of the entity
   *
   *  @return the view matrix
   */
  geom::Matrix modelMatrix() const;

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

  const image::Image& contents() const;

  void setContents(image::Image image);

  void mergeProperties(const Entity& entity, PropertyMaskType only);

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

 protected:
  core::Name _identifier;
  geom::Rect _bounds;
  geom::Point _position;
  geom::Point _anchorPoint;
  geom::Matrix _transformation;
  Color _backgroundColor;
  image::Image _contents;
  double _opacity;

  void notifyInterfaceIfNecessary(
      Property property,
      core::Name identifier = core::Name() /* dead name */) const;

 private:
  UpdateCallback _updateCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(Entity);
};

}  // namespace entity
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_ENTITY_
