/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace type {

class AttributedStringBuilder {
 public:
  AttributedStringBuilder();

  ~AttributedStringBuilder();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AttributedStringBuilder);
};

}  // namespace type
}  // namespace rl
