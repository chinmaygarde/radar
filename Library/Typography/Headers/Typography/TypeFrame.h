/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace type {

class TypeFrame {
 public:
  TypeFrame();

  TypeFrame(TypeFrame&&);

  ~TypeFrame();

  bool isValid() const;

 private:
  bool _valid;

  RL_DISALLOW_COPY_AND_ASSIGN(TypeFrame);
};

}  // namespace type
}  // namespace rl
