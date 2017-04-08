/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>

namespace rl {
namespace type {

class Font {
 public:
  Font(core::URI uri);

  ~Font();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Font);
};

}  // namespace type
}  // namespace rl
