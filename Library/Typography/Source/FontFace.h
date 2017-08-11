/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Typography/Font.h>
#include <string>
#include "HarfbuzzHelpers.h"

namespace rl {
namespace type {

class FontFace {
 public:
  FontFace(const core::URI& uri, size_t index);

  ~FontFace();

  bool isValid() const;

  hb_face_t* face() const;

  size_t glyphCount() const;

 private:
  HBFacePtr _face;
  bool _valid = false;

  RL_DISALLOW_COPY_AND_ASSIGN(FontFace);
};

}  // namespace type
}  // namespace rl
