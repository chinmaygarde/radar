/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <string>

namespace rl {
namespace type {

class TypeFrame {
 public:
  TypeFrame(const std::string& string);

  ~TypeFrame();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TypeFrame);
};

}  // namespace type
}  // namespace rl
