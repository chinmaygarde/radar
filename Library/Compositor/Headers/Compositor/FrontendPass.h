// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <vector>

namespace rl {
namespace compositor {

class BackEndPass;
class Frame;
class Primitive;

class FrontEndPass {
 public:
  FrontEndPass();

  ~FrontEndPass();

  FrontEndPass(FrontEndPass&&);

  bool hasRenderables() const;

  size_t primitivesCount() const;

  void addPrimitive(std::shared_ptr<Primitive> primitive);

 private:
  std::vector<std::shared_ptr<Primitive>> _primitives;

  /*
   *  This will be removed once the pass IR is formalized.
   */
  friend class BackEndPass;
  bool prepareInBackendPass(BackEndPass& pass);
  bool renderInBackEndPass(const BackEndPass& pass, Frame& frame) const;

  RL_DISALLOW_COPY_AND_ASSIGN(FrontEndPass);
};

}  // namespace compositor
}  // namespace rl
