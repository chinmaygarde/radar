/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <Typography/AttributedString.h>
#include <Typography/Font.h>
#include <Typography/FontLibrary.h>
#include <Typography/TextRun.h>
#include <Typography/Types.h>

namespace rl {
namespace type {

class ShapedTextRun {
 public:
  ShapedTextRun();

  ShapedTextRun(const String& string,
                const TextRun& run,
                const FontLibrary& library);

  ShapedTextRun(ShapedTextRun&&);

  ~ShapedTextRun();

  bool isValid() const;

  geom::Size size() const;

  size_t glyphCount() const;

 private:
  HBRef<hb_buffer_t> _buffer = {nullptr, HBRefDeleterNull};
  Font _font;
  bool _valid = false;

  RL_DISALLOW_COPY_AND_ASSIGN(ShapedTextRun);
};

}  // namespace type
}  // namespace rl
