/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>

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

  void* handle() const;

 private:
  void* __opaque = nullptr;
  bool _valid = false;

  RL_DISALLOW_COPY_AND_ASSIGN(Font);
};

}  // namespace type
}  // namespace rl
