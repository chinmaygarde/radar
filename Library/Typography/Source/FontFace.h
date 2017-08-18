/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Typography/Font.h>
#include <Typography/Types.h>
#include <string>

namespace rl {
namespace type {

class FontFace {
 public:
  FontFace(const core::URI& uri, size_t index);

  ~FontFace();

  bool isValid() const;

  size_t glyphCount() const;

  hb_face_t* handle() const;

 private:
  HBRef<hb_face_t> _face;

  RL_DISALLOW_COPY_AND_ASSIGN(FontFace);
};

}  // namespace type
}  // namespace rl
