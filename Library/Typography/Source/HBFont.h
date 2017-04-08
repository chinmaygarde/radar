/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Typography/Types.h>
#include <hb.h>
#include "FTFace.h"

namespace rl {
namespace type {

class HBFont {
 public:
  HBFont();

  HBFont(FTFaceAccess& face);

  HBFont(HBFont&&);

  ~HBFont();

  hb_font_t* handle() const;

  bool isValid() const;

  Point pointSize() const;

  void setPointSize(Point size);

 private:
  hb_font_t* _font;

  RL_DISALLOW_COPY_AND_ASSIGN(HBFont);
};

}  // namespace type
}  // namespace rl
