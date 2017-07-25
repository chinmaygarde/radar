/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ColoredBoxStrokePrimitive.h"

namespace rl {
namespace compositor {

ColoredBoxStrokePrimitive::ColoredBoxStrokePrimitive(entity::Color color,
                                                     double size)
    : _color(color), _size(size) {}

ColoredBoxStrokePrimitive::~ColoredBoxStrokePrimitive() = default;

bool ColoredBoxStrokePrimitive::prepareToRender(BackEndPass& backEndPass) {
  RL_WIP;
  return false;
}

bool ColoredBoxStrokePrimitive::render(Frame& frame) const {
  RL_WIP;
  return false;
}

}  // namespace compositor
}  // namespace rl
