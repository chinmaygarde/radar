/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace type {

class AttributedString {
 public:
  AttributedString();

  ~AttributedString();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AttributedString);
};

}  // namespace type
}  // namespace rl
