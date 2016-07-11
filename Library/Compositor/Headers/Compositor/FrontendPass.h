// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_FRONTENDPASS_H_
#define RADAR_COMPOSITOR_FRONTENDPASS_H_

#include <Core/Macros.h>
#include <Compositor/Primitive/Primitive.h>

#include <vector>

namespace rl {
namespace compositor {

class BackEndPass;
class Frame;

class FrontEndPass {
 public:
  FrontEndPass();

  ~FrontEndPass();

  FrontEndPass(FrontEndPass&&);

  size_t primitivesCount() const;

  void addPrimitive(std::shared_ptr<Primitive> primitive);

  bool prepareInBackendPass(BackEndPass& pass);

  bool render(const BackEndPass& pass, Frame& frame) const;

 private:
  std::vector<std::shared_ptr<Primitive>> _primitives;

  RL_DISALLOW_COPY_AND_ASSIGN(FrontEndPass);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_FRONTENDPASS_H_
