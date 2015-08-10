// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PATCHCHUNK__
#define __RADARLOVE_COMPOSITOR_PATCHCHUNK__

#include <Core/Core.h>
#include <Geometry/Rect.h>
#include <Geometry/Matrix.h>
#include <Compositor/Color.h>

namespace rl {
struct PatchChunk {
  using Identifier = uintptr_t;

  enum Command {
    Nothing = 0,
    MakeRoot,
    Created,
    Destroyed,
    AddedTo,
    RemovedFrom,
    Bounds,
    Position,
    AnchorPoint,
    Color,
    Matrix,
    Opacity,
    Sentinel,
  };

  struct MakeRootCommandData {
    /*
     *  Nothing
     */
  };

  struct CreatedCommandData {
    /*
     *  Nothing
     */
  };

  struct DestroyedCommandData {
    /*
     *  Nothing
     */
  };

  struct AddedToCommandData {
    Identifier layer;
  };

  struct RemovedFromCommandData {
    Identifier layer;
  };

  struct BoundsCommandData {
    Rect bounds;
  };

  struct PositionCommandData {
    Point position;
  };

  struct AnchorPointCommandData {
    Point anchorPoint;
  };

  struct ColorCommandData {
    rl::Color color;
  };

  struct MatrixCommandData {
    rl::Matrix matrix;
  };

  struct OpacityCommandData {
    double opacity;
  };

  Command command;
  Identifier target;
  union {
    CreatedCommandData createdData;
    DestroyedCommandData destroyedData;
    AddedToCommandData addedToData;
    RemovedFromCommandData removedFromData;
    BoundsCommandData boundsData;
    PositionCommandData positionData;
    AnchorPointCommandData anchorPointData;
    ColorCommandData colorData;
    MatrixCommandData matrixData;
    OpacityCommandData opacityData;
  } data;
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_PATCHCHUNK__) */
