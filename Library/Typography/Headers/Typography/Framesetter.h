/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <Typography/AttributedString.h>

namespace rl {
namespace type {

class Framesetter {
 public:
  Framesetter(AttributedString string, geom::Size frameSize);

  ~Framesetter();

  bool isValid() const;

 private:
  bool _valid = false;

  RL_DISALLOW_COPY_AND_ASSIGN(Framesetter);
};

}  // namespace type
}  // namespace rl
