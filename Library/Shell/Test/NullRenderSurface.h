/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Shell/Shell.h>

namespace rl {
namespace shell {

class NullRenderSurface : public coordinator::RenderSurface {
 public:
  NullRenderSurface();

  bool makeCurrent() override;

  bool present() override;

  void accessWillBegin() override;

  void accessDidEnd() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullRenderSurface);
};

}  // namespace shell
}  // namespace rl
