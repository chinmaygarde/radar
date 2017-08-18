/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Typography/Types.h>

namespace rl {
namespace type {

class FontFace;

class Font {
 public:
  Font();

  Font(const FontFace& fontFace, double size);

  Font(Font&&);

  ~Font();

  Font& operator=(Font&&);

  double size() const;

  bool isValid() const;

  std::string postscriptName() const;

  hb_font_t* handle() const;

 private:
  HBRef<hb_font_t> _handle{nullptr, HBRefDeleterNull};

  RL_DISALLOW_COPY_AND_ASSIGN(Font);
};

}  // namespace type
}  // namespace rl
