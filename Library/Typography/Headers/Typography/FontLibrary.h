/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <map>
#include <memory>

namespace rl {
namespace type {

class FontFace;

class FontLibrary {
 public:
  FontLibrary();

  ~FontLibrary();

  bool registerFont(const core::URI& fontFileName, size_t index);

  size_t registeredFonts() const;

 private:
  std::map<std::string, std::unique_ptr<FontFace>> _registeredFonts;

  RL_DISALLOW_COPY_AND_ASSIGN(FontLibrary);
};

}  // namespace type
}  // namespace rl
